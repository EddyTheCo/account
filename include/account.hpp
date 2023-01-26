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
    Account(quint32 coin=4219);
    QString seed(void)const{return QString(seed_.toHex());}
    void set_seed(QString seedstr);
    address_bundle get_addr(const QVector<quint32>& subpath) const; //(0,0,0)
signals:
    void seedChanged();
private:
    void set_random_seed(void);
    Master_key master_;
    QVector<quint32> path_;
    QByteArray seed_;
};

