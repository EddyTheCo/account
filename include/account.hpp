#pragma once
#include"crypto/qslip10.hpp"
#include"block/qaddresses.hpp"
#include"encoding/qbech32.hpp"
#include<QObject>
#include<QJsonObject>
#include<QString>
#include<QByteArray>

#if defined(USE_QML)
#include <QtQml/qqmlregistration.h>
#endif

#include <QtCore/QtGlobal>
#if defined(WINDOWS_ACCOU)
# define ACCOU_EXPORT Q_DECL_EXPORT
#else
#define ACCOU_EXPORT Q_DECL_IMPORT
#endif

using namespace qiota;
using namespace qiota::qblocks;
using namespace qcrypto;

class ACCOU_EXPORT Account : public QObject
{

    Q_OBJECT
#if defined(USE_QML)
    Q_PROPERTY(QString  seed READ seed WRITE setSeed NOTIFY seedChanged)
    Q_PROPERTY(Account  instance READ instance NOTIFY instanceChanged)
    QML_ELEMENT
    QML_SINGLETON
#endif
    static QByteArray setRandomSeed(void);
    Account(QObject *parent = nullptr,QByteArray seed=setRandomSeed());
public:
    static Account* instance();
    QString seed(void)const{return QString(seed_.toHex());}
    void setSeed(QString seedstr);
    std::pair<QByteArray,QByteArray> getKeys(const QVector<quint32>& subpath); //(0,0,0)
    std::shared_ptr<Address> getAddr(const QVector<quint32> subpath)
    {
        return std::shared_ptr<Address>
            (new Ed25519_Address(QCryptographicHash::hash(getKeys(subpath).first,QCryptographicHash::Blake2b_256)));
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
    Q_INVOKABLE void set_path(const QVector<quint32>& path_m){path_=path_m;}
    Q_INVOKABLE QJsonObject path_json(const QVector<quint32> subpath)
    {
        return getAddr(subpath)->get_Json();
    };
    Q_INVOKABLE QJsonObject bech32Json(const QString &bech32addr)
    {
        auto addr=qencoding::qbech32::Iota::decode(bech32addr);
        if(addr.second.size())
            return Address::from_array(addr.second)->get_Json();
        return QJsonObject();
    };
    Q_INVOKABLE QString jsonBech32(const QJsonValue &json_addr,const QString& hrp)
    {
        const auto addr=Address::from_(json_addr)->addr();
        return qencoding::qbech32::Iota::encode(hrp,addr);
    };
signals:
    void seedChanged();
    void instanceChanged();
private:

    Master_key master_;
    QByteArray seed_;
    QVector<quint32> path_;
    static Account * m_instance;

};

