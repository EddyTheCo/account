
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"account.hpp"
#include <QCryptographicHash>


Account* Account::m_instance=nullptr;
Account* Account::instance()
{
    if (!m_instance) m_instance=new Account();
    return m_instance;
}
QByteArray Account::setRandomSeed(void){
    auto seed=QByteArray(32,0);
    auto buffer=QDataStream(&seed,QIODevice::WriteOnly | QIODevice::Append);
    for(auto i=0;i<8;i++)
    {
        quint32 value = QRandomGenerator::global()->generate();
        buffer<<value;
    }
    return seed;
}
void Account::setSeed(QString seedstr)
{
    auto var=QByteArray::fromHex(seedstr.toUtf8());
    if(var.size()>=32&&seedstr!=Account::instance()->seed())
    {
        emit seedChanged();
        m_instance=new Account(Account::instance()->parent(),var);
        deleteLater();
    }
}
Account::Account(QObject *parent,QByteArray seed):QObject(parent),master_(seed),path_({44,4219})
{
}
std::pair<QByteArray,QByteArray> Account::getKeys(const QVector<quint32>& subpath)
{
    QVector<quint32> fullpath=path_;
    fullpath.append(subpath);
    auto keys=master_.slip10_key_from_path(fullpath);
    return qed25519::create_keypair(keys.secret_key());
}

