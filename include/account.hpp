#pragma once
#include<QObject>
#include<QString>
#include<QByteArray>

#include"crypto/qslip10.hpp"
#include"block/qaddresses.hpp"
#include"encoding/qbech32.hpp"

#include"vault.hpp"

#if defined(USE_QML)
#include<QtQml>
#endif

#include <QtCore/QtGlobal>
#if defined(WINDOWS_ACCOU)
# define ACCOU_EXPORT Q_DECL_EXPORT
#else
#define ACCOU_EXPORT Q_DECL_IMPORT
#endif

namespace qiota{


using namespace qblocks;
using namespace qcrypto;

class ACCOU_EXPORT Account : public QObject
{
    qcrypto::Master_key m_master;
    QByteArray m_seed;
    QVector<quint32> m_path;
    static Account * m_instance;
    QString m_sentence;
    bool m_mnemonicMode;
    qutils::Vault* m_vault;

    Q_OBJECT
#if defined(USE_QML)
    Q_PROPERTY(QString  seed READ seed WRITE setSeed NOTIFY changed)
    Q_PROPERTY(bool  mnmonicMode MEMBER m_mnemonicMode NOTIFY modeChanged)
    Q_PROPERTY(QVector<quint32> path MEMBER m_path NOTIFY changed)
    Q_PROPERTY(bool isVaultEmpty READ getIsVaultEmpty NOTIFY isVaultEmptyChanged)
    QML_ELEMENT
    QML_SINGLETON
#endif
    static std::pair<QByteArray,QString> setRandomSeed();
    bool setSeedfromRaw(QByteArray seed);
    Account(QObject *parent = nullptr, std::pair<QByteArray,QString> mnemonicpair=setRandomSeed());
public:
    static Account* instance();
    void setVaultFile(QString filename){m_vault->setFile(filename);};
    bool getIsVaultEmpty()const{return m_vault->isEmpty();}
    Q_INVOKABLE bool readFromVault(QString password);
    Q_INVOKABLE bool writeToVault(QString password);
    Q_INVOKABLE bool changeVaultPassword(QString oldPass,QString newPass);

    void setMnemonicMode(bool mode){if(mode!=m_mnemonicMode){m_mnemonicMode=mode;emit modeChanged();}}
#if defined(USE_QML)
    static Account *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        return instance();
    }
#endif
    QString seed(void)const{return (m_mnemonicMode)?m_sentence:QString(m_seed.toHex());}
    Q_INVOKABLE void setSeed(QString seedstr);
    Q_INVOKABLE void setPath(const QVector<quint32>& path);
    std::pair<QByteArray,QByteArray> getKeys(const QVector<quint32>& subpath); //(0,0,0)
    std::shared_ptr<const Address> getAddr(const QVector<quint32> subpath)
    {
        return Address::Ed25519(QCryptographicHash::hash(getKeys(subpath).first,QCryptographicHash::Blake2b_256));
    }
    c_array getAddrArray(const QVector<quint32> subpath)
    {
        return getAddr(subpath)->addr();
    }
    Q_INVOKABLE QString getAddrHex(const QVector<quint32> subpath)
    {
        return getAddrArray(subpath).toHexString();
    }

    Q_INVOKABLE QString getAddrBech32(const QVector<quint32> subpath,QString hrp)
    {
        return qencoding::qbech32::Iota::encode(hrp,getAddrArray(subpath));
    };

signals:
    void changed();
    void modeChanged();
    void isVaultEmptyChanged();

};

};
