#pragma once
#include"qaddr_bundle.hpp"
#include"crypto/qslip10.hpp"
#include<QObject>
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
    {return get_addr(subpath).get_address<qblocks::Address::Ed25519_typ>().toHexString();}
    Q_INVOKABLE static QString addr_bech32(const QVector<quint32> subpath,QString hrp)
    {return get_addr(subpath).get_address_bech32<qblocks::Address::Ed25519_typ>(hrp);};
    Q_INVOKABLE static void set_path(const QVector<quint32>& path_m){path_=path_m;}
signals:
    void seedChanged();
private:
    void set_random_seed(void);
    static Master_key master_;
    static QByteArray seed_;
    static QVector<quint32> path_;

};

