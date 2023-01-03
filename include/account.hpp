#pragma once
#include"encoding/qbech32.hpp"
#include"crypto/qslip10.hpp"
#include"block/qblock.hpp"
#include"client/qclient.hpp"
using namespace qcrypto;
using namespace qiota;
class address_bundle
{
public:
    address_bundle(const std::pair<QByteArray,QByteArray>& key_pair_m,QString hrp_m="rms");
    const std::pair<QByteArray,QByteArray> key_pair;
    const QString hrp;
    const qblocks::c_array addr_hash;
    const QString addr;

    void consume_outputs(std::vector<Node_output> outs_,const quint64 amount_need_it,
                         qblocks::c_array& Inputs_Commitments, quint64& amount,
                         std::vector<std::shared_ptr<qblocks::Output>>& ret_outputs,
                         std::vector<std::shared_ptr<qblocks::Input>>& inputs);
    quint16 reference_count(void)const{return reference_count_;}
    qblocks::signature sign(const QByteArray & message)const;
    std::shared_ptr<qblocks::Signature> signature(const QByteArray & message)const;
    std::shared_ptr<qblocks::Unlock> signature_unlock(const QByteArray & message)const;
    void create_unlocks(const QByteArray & message,std::vector<std::shared_ptr<qblocks::Unlock>>& unlocks)const;

private:

    qblocks::c_array get_hash(void)const;
    quint16 reference_count_;
    QString get_address(void)const;
};
class account
{

private:
    QByteArray get_random_seed(void)const;
public:
    account();
    address_bundle get_payment_addr(quint32 index) const;
    address_bundle get_payment_addr(void) const;
    address_bundle get_publish_addr(void) const;

    void next_payment_addr(void){last_index++;}


private:
    const Master_key master_;
    quint32 last_index;

    const QVector<quint32> publish_path;
};

