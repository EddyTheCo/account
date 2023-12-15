#include"account.hpp"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"crypto/qbip39.hpp"

namespace qiota{


Account* Account::m_instance=nullptr;
Account* Account::instance()
{
    if (!m_instance) m_instance=new Account();
    return m_instance;
}

void Account::setPath(const QVector<quint32>& path)
{
    if(path!=m_path)
    {
        m_path=path;
        emit changed();
    }
}
void Account::setSeed(QString seedstr)
{

    if(m_mnemonicMode)
    {
        const auto stringList= seedstr.split(QChar::Space);
        if(stringList.size()>11)
        {
            const auto mnemonic=Mnemonic<Language::en>(stringList);
            if(mnemonic.isValid())
            {
                const auto varseed=mnemonic.getSeed();
                if(varseed!=m_seed)
                {
                    m_seed=varseed;
                    m_sentence=mnemonic.m_words.join(QChar::Space);
                    m_master=Master_key(m_seed);
                    emit changed();
                }
            }
        }
    }
    else
    {
        auto varseed=QByteArray::fromHex(seedstr.toUtf8());
        if(varseed.size()>=32&&varseed!=m_seed)
        {
            m_seed=varseed;
            m_master=Master_key(m_seed);
            m_sentence=m_seed.toHex();
            emit changed();
        }
    }

}
std::pair<QByteArray,QString> Account::setRandomSeed(){
    const auto mnemonic=Mnemonic<Language::en>();

    return std::make_pair(mnemonic.getSeed(),mnemonic.m_words.join(QChar::Space));
}
Account::Account(QObject *parent,std::pair<QByteArray,QString> mnemonicpair):QObject(parent),m_mnemonicMode(true),m_seed(mnemonicpair.first),m_master(mnemonicpair.first),m_sentence(mnemonicpair.second),m_path({44,4219})
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
