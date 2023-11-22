#pragma once
#include"crypto/qslip10.hpp"
#include"block/qaddresses.hpp"
#include"encoding/qbech32.hpp"
#include"crypto/qed25519.hpp"
#include<QObject>
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

namespace qiota{


using namespace qblocks;
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
    static QByteArray setRandomSeed(quint8 byteNum=8);
    Account(QObject *parent = nullptr,QByteArray seed=setRandomSeed());
public:
    static Account* instance();
    QString seed(void)const{return QString(m_seed.toHex());}
    void setSeed(QString seedstr);
    void setPath(const QVector<quint32>& path);
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

private:
    qcrypto::Master_key m_master;
    QByteArray m_seed;
    QVector<quint32> m_path;
    static Account * m_instance;

};

};
