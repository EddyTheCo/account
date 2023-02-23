
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"account.hpp"
#include <QCryptographicHash>



void Account::set_random_seed(void){
    auto buffer=QDataStream(&seed_,QIODevice::WriteOnly | QIODevice::Append);
	for(auto i=0;i<8;i++)
	{
		quint32 value = QRandomGenerator::global()->generate();
		buffer<<value;
    }
}
Account::Account(quint32 coin):master_(QByteArray(32,0)),path_(QVector<quint32>{44,coin})
{
    set_random_seed();
    master_=Master_key(seed_);

}
address_bundle Account::get_addr(const QVector<quint32>& subpath) const
{
    QVector<quint32> full_path=path_;
    full_path.append(subpath);
    auto keys=master_.slip10_key_from_path(full_path);
	return address_bundle(qed25519::create_keypair(keys.secret_key()));
}
void Account::set_seed(QString seedstr)
{

    auto var=QByteArray::fromHex(seedstr.toUtf8());
    if(var.size()>=32&&var!=seed_)
    {
        seed_=var;
        master_=Master_key(seed_);
        emit seedChanged();
    }
}
