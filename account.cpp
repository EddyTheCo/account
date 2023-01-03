
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"account.hpp"
#include <QCryptographicHash>

address_bundle::address_bundle(const std::pair<QByteArray,QByteArray>& key_pair_m,QString hrp_m):key_pair(key_pair_m),
    hrp(hrp_m), addr_hash(get_hash()), addr(get_address()), reference_count_(0)
{ };


qblocks::c_array address_bundle::get_hash(void)const
{
	const auto hash=qblocks::c_array(QCryptographicHash::hash(key_pair.first,QCryptographicHash::Blake2b_256));
	return hash;
}

QString address_bundle::get_address(void)const
{
	auto pub=addr_hash;
	pub.push_front('\x00');
	const auto addr=qencoding::qbech32::Iota::encode(hrp,pub);
	return addr;
}

QByteArray account::get_random_seed(void)const{
	QByteArray var;
	auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
	for(auto i=0;i<8;i++)
	{
		quint32 value = QRandomGenerator::global()->generate();
		buffer<<value;
	}
    //return QByteArray::fromHex("20b5f5a1dc76bc385d27fe9de15d97c0f338ff1389ecae3f7c9bd748707e141e");
    return var;
}
account::account():master_(get_random_seed()),publish_path({44,4219,0,0,0}),last_index(0)
{
}
address_bundle account::get_payment_addr(void) const
{
	QVector<quint32> path={44,4219,1,0};
	path.push_back(last_index);
	auto keys=master_.slip10_key_from_path(path);
	return address_bundle(qed25519::create_keypair(keys.secret_key()));
}
address_bundle account::get_payment_addr(quint32 index) const
{
    QVector<quint32> path={44,4219,1,0};
    path.push_back(index);
    auto keys=master_.slip10_key_from_path(path);
    return address_bundle(qed25519::create_keypair(keys.secret_key()));
}
address_bundle account::get_publish_addr(void) const
{
	auto keys=master_.slip10_key_from_path(publish_path);
	const auto edkeys=qed25519::create_keypair(keys.secret_key());
	return address_bundle(edkeys);
}

qblocks::signature address_bundle::sign(const QByteArray & message)const
{
	return qblocks::signature(qed25519::sign(key_pair,message));
}
std::shared_ptr<qblocks::Signature> address_bundle::signature(const QByteArray & message)const
{
    return std::shared_ptr<qblocks::Signature>
            (new qblocks::Ed25519_Signature(qblocks::public_key(key_pair.first),
                                   sign(message)));
}
std::shared_ptr<qblocks::Unlock> address_bundle::signature_unlock(const QByteArray & message)const
{
    return std::shared_ptr<qblocks::Unlock>(new qblocks::Signature_Unlock(signature(message)));
}
void address_bundle::create_unlocks(const QByteArray & message,std::vector<std::shared_ptr<qblocks::Unlock>>& unlocks)const
{
    if(reference_count_)
    {
        const quint16 reference_index=unlocks.size();
        unlocks.push_back(signature_unlock(message));
        for(auto i=0;i<reference_count_-1;i++)
        {
            unlocks.push_back(std::shared_ptr<qblocks::Unlock>(new qblocks::Reference_Unlock(reference_index)));
        }
    }
}
void address_bundle::consume_outputs(std::vector<Node_output> outs_,const quint64 amount_need_it,
                                                                         qblocks::c_array& Inputs_Commitments, quint64& amount,
                                                                         std::vector<std::shared_ptr<qblocks::Output>>& ret_outputs,
                            std::vector<std::shared_ptr<qblocks::Input>>& inputs)
{

    const auto cday=QDateTime::currentDateTime().toSecsSinceEpoch();

    while(((amount_need_it)?amount<amount_need_it:1)&&!outs_.empty())
    {
        const auto v=outs_.back();
        if(!v.metadata().is_spent_&&v.output()->type_m==3)
        {

            const auto basic_output_=std::dynamic_pointer_cast<qblocks::Basic_Output>(v.output());

            const auto  stor_unlock=basic_output_->get_unlock_(1);
            quint64 ret_amount=0;
            if(stor_unlock)
            {
                const auto sdruc=std::dynamic_pointer_cast<qblocks::Storage_Deposit_Return_Unlock_Condition>(stor_unlock);
                ret_amount=sdruc->return_amount();
                const auto ret_address=sdruc->return_address();
                const auto retUnlcon=std::shared_ptr<qblocks::Unlock_Condition>(new qblocks::Address_Unlock_Condition(ret_address));
                const auto retOut= std::shared_ptr<qblocks::Output>(new qblocks::Basic_Output(ret_amount,{retUnlcon},{},{}));
                ret_outputs.push_back(retOut);
            }
            const auto expir=basic_output_->get_unlock_(3);
            if(expir)
            {
                const auto expiration_cond=*(std::dynamic_pointer_cast<qblocks::Expiration_Unlock_Condition>(expir));
                const auto unix_time=expiration_cond.unix_time();

                if(cday+100>unix_time)
                {
                    outs_.pop_back();
                    continue;
                }
            }
            const auto time_lock=basic_output_->get_unlock_(2);
            if(time_lock)
            {
                const auto time_lock_cond=*(std::dynamic_pointer_cast<qblocks::Timelock_Unlock_Condition>(time_lock));
                const auto unix_time=time_lock_cond.unix_time();
                if(cday<unix_time)
                {
                    outs_.pop_back();
                    continue;
                }

            }

            inputs.push_back(std::shared_ptr<qblocks::Input>(new qblocks::UTXO_Input(v.metadata().transaction_id_,
                                                                                     v.metadata().output_index_)));
            qblocks::c_array prevOutputSer;
            prevOutputSer.from_object<qblocks::Output>(*(v.output()));
            auto Inputs_Commitment1=QCryptographicHash::hash(prevOutputSer, QCryptographicHash::Blake2b_256);
            Inputs_Commitments.append(Inputs_Commitment1);
            amount+=basic_output_->amount()-ret_amount;
            reference_count_++;
        }
        outs_.pop_back();
    }
}
