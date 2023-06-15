#pragma once
#include"qaddr_bundle.hpp"
#include"crypto/qslip10.hpp"
#include<QObject>
#include<QJsonObject>
#include<QString>
#include<QByteArray>
#include <QtQml/qqmlregistration.h>

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
				return Address::from_array(addr.second)->get_Json();
			return QJsonObject();
		};
		Q_INVOKABLE static QString json_bech32(const QJsonValue &json_addr,const QString& hrp)
		{
			const auto addr=Address::from_(json_addr)->addr();
			return qencoding::qbech32::Iota::encode(hrp,addr);
		};
signals:
		void seedChanged();
	private:
		void set_random_seed(void);
		static Master_key master_;
		static QByteArray seed_;
		static QVector<quint32> path_;

};

