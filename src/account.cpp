#include"account.hpp"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"crypto/qbip39.hpp"

namespace qiota{


Account* Account::m_instance=nullptr;
Account* Account::instance()
{
    if (!m_instance)
    {
        m_instance=new Account();
#if defined(USE_QML)
        QJSEngine::setObjectOwnership(m_instance,QJSEngine::CppOwnership);
#endif
    }
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
            const auto mnemonic=Mnemonic<Language::En>(stringList);
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
        auto seed=QByteArray::fromHex(seedstr.toUtf8());
        setSeedfromRaw(seed);
    }

}
bool Account::setSeedfromRaw(QByteArray seed)
{
    if(seed.size()>=32&&seed!=m_seed)
    {
        m_seed=seed;
        m_master=Master_key(m_seed);
        m_sentence=m_seed.toHex();
        emit changed();
        return true;
    }
    return false;

}
std::pair<QByteArray,QString> Account::setRandomSeed(){
    const auto mnemonic=Mnemonic<Language::En>();

    return std::make_pair(mnemonic.getSeed(),mnemonic.m_words.join(QChar::Space));
}
Account::Account(QObject *parent,std::pair<QByteArray,QString> mnemonicpair):QObject(parent),
    m_mnemonicMode(true),
    m_seed(mnemonicpair.first),
    m_master(mnemonicpair.first),
    m_sentence(mnemonicpair.second),
    m_path({44,4219}),m_vault(new qutils::Vault(this))
{
    connect(m_vault,&qutils::Vault::isEmptyChanged,this,&Account::isVaultEmptyChanged);
}
bool Account::changeVaultPassword(QString oldPass,QString newPass)
{
    return m_vault->changePassword(oldPass,newPass);
}
bool Account::writeToVault(QString password)
{
    return m_vault->setData(m_seed,password.toUtf8());
}
bool Account::readFromVault(QString password)
{
    if(m_vault->isEmpty()) return false;

    const auto seed=m_vault->getData(password.toUtf8());
    m_mnemonicMode=false;

    return setSeedfromRaw(seed);

}
std::pair<QByteArray,QByteArray> Account::getKeys(const QVector<quint32>& subpath)
{
    QVector<quint32> fullpath=m_path;
    fullpath.append(subpath);
    auto keys=m_master.slip10_key_from_path(fullpath);
    return qed25519::create_keypair(keys.secret_key());
}

}
