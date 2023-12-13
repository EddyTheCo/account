#pragma once
#include<QObject>
#include<QString>
#include<QByteArray>

#include"crypto/qslip10.hpp"
#include"block/qaddresses.hpp"
#include"encoding/qbech32.hpp"


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

    Q_OBJECT
#if defined(USE_QML)
    Q_PROPERTY(QString  seed READ seed WRITE setSeed NOTIFY Changed)
    Q_PROPERTY(QVector<quint32> path MEMBER m_path NOTIFY Changed)
    QML_ELEMENT
    QML_SINGLETON
#endif
    static QByteArray setRandomSeed(quint8 byte4Num=8);
    Account(QObject *parent = nullptr,QByteArray seed=setRandomSeed());
public:
    static Account* instance();
#if defined(USE_QML)
    static Account *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        return instance();
    }
#endif
    QString seed(void)const{return QString(m_seed.toHex());}
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
    void Changed();


};

};
