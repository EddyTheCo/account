#include"account.hpp"
#include <QCryptographicHash>
#include <QRandomGenerator>

namespace qiota{


Account* Account::m_instance=nullptr;
Account* Account::instance()
{
    if (!m_instance) m_instance=new Account();
    return m_instance;
}

QByteArray Account::setRandomSeed(quint8 byteNum){

    auto seed=QByteArray(32,0);
    auto buffer=QDataStream(&seed,QIODevice::WriteOnly | QIODevice::Append);
    for(auto i=0;i<byteNum;i++)
    {
        quint32 value = QRandomGenerator::global()->generate();
        buffer<<value;
    }
    return seed;
}
void Account::setPath(const QVector<quint32>& path)
{
    if(path!=m_path)
    {
        m_path=path;
        emit Changed();
    }
}
void Account::setSeed(QString seedstr)
{
    auto var=QByteArray::fromHex(seedstr.toUtf8());
    if(var.size()>=32&&var!=m_seed)
    {
        m_seed=var;
        m_master=Master_key(m_seed);
        emit Changed();
    }
}
Account::Account(QObject *parent,QByteArray seed):QObject(parent),m_master(seed),m_path({44,4219})
{

}
std::pair<QByteArray,QByteArray> Account::getKeys(const QVector<quint32>& subpath)
{
    QVector<quint32> fullpath=m_path;
    fullpath.append(subpath);
    auto keys=m_master.slip10_key_from_path(fullpath);
    return qed25519::create_keypair(keys.secret_key());
}

}
