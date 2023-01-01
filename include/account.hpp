#pragma once

#include"crypto/qslip10.hpp"
#include"block/qblock.hpp"
using namespace qcrypto;
using namespace qiota;
class address_bundle
{
	public:
		address_bundle(const std::pair<QByteArray,QByteArray>& key_pair_m,QString hrp_m="rms");
		const std::pair<QByteArray,QByteArray> key_pair;
		const QString hrp;
		const qblocks::c_array addr_hash;
		const QString addr;
	
		qblocks::signature sign(const QByteArray & message)const;

	private:

		qblocks::c_array get_hash(void)const;

		QString get_address(void)const;
};
class account
{

	private:
		QByteArray get_random_seed(void)const;
	public:
		account();
        address_bundle get_payment_addr(quint32 index) const;
        address_bundle get_payment_addr(void) const;
		address_bundle get_publish_addr(void) const;

		void next_payment_addr(void){last_index++;}


	private:
		const Master_key master_;
		quint32 last_index;

		const QVector<quint32> publish_path;
};

