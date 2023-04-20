#pragma once
#include"qaddr_bundle.hpp"
#include"crypto/qslip10.hpp"
#include<QObject>
#include<QJsonObject>
#include<QString>
#include<QByteArray>
#include <QtQml/qqmlregistration.h>
using namespace qiota;
using namespace qcrypto;

class Account : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString  seed READ seed WRITE set_seed NOTIFY seedChanged)

    QML_ELEMENT
    QML_SINGLETON

public:
    Account(QObject *parent = nullptr);
    QString seed(void)const{return QString(seed_.toHex());}
    void set_seed(QString seedstr);
    static address_bundle get_addr(const QVector<quint32>& subpath); //(0,0,0)
    Q_INVOKABLE static QString addr(const QVector<quint32> subpath)
    {return get_addr(subpath).get_address()->addr().toHexString();}
    Q_INVOKABLE static QString addr_bech32(const QVector<quint32> subpath,QString hrp)
    {return get_addr(subpath).get_address_bech32(hrp);};
    Q_INVOKABLE static void set_path(const QVector<quint32>& path_m){path_=path_m;}
    Q_INVOKABLE static QJsonObject path_json(const QVector<quint32> subpath)
    {
        return get_addr(subpath).get_address()->get_Json();
    };
    Q_INVOKABLE static QJsonObject bech32_json(const QString &bech32addr)
    {
        auto addr=qencoding::qbech32::Iota::decode(bech32addr);
        if(addr.second.size())
        return qblocks::Address::from_(addr.second)->get_Json();
        return QJsonObject();
    };
signals:
    void seedChanged();
private:
    void set_random_seed(void);
    static Master_key master_;
    static QByteArray seed_;
    static QVector<quint32> path_;

};

