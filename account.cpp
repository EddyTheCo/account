
#include <QRandomGenerator>
#include"crypto/qed25519.hpp"
#include"account.hpp"
#include <QCryptographicHash>

address_bundle::address_bundle(const std::pair<QByteArray,QByteArray>& key_pair_m,QString hrp_m):key_pair(key_pair_m),
	hrp(hrp_m), addr_hash(get_hash()), addr(get_address())
{ };


qblocks::c_array address_bundle::get_hash(void)const
{
	const auto hash=qblocks::c_array(QCryptographicHash::hash(key_pair.first,QCryptographicHash::Blake2b_256));
	return hash;
}

QString address_bundle::get_address(void)const
{
	auto pub=addr_hash;
	pub.push_front('\x00');
	const auto addr=qencoding::qbech32::Iota::encode(hrp,pub);
	return addr;
}

QByteArray account::get_random_seed(void)const{
	QByteArray var;
	auto buffer=QDataStream(&var,QIODevice::WriteOnly | QIODevice::Append);
	for(auto i=0;i<8;i++)
	{
		quint32 value = QRandomGenerator::global()->generate();
		buffer<<value;
	}
    //return QByteArray::fromHex("20b5f5a1dc76bc385d27fe9de15d97c0f338ff1389ecae3f7c9bd748707e141e");
    return var;
}
account::account():master_(get_random_seed()),publish_path({44,4219,0,0,0}),last_index(0)
{
}
address_bundle account::get_payment_addr(void) const
{
	QVector<quint32> path={44,4219,1,0};
	path.push_back(last_index);
	auto keys=master_.slip10_key_from_path(path);
	return address_bundle(qed25519::create_keypair(keys.secret_key()));
}
address_bundle account::get_payment_addr(quint32 index) const
{
    QVector<quint32> path={44,4219,1,0};
    path.push_back(index);
    auto keys=master_.slip10_key_from_path(path);
    return address_bundle(qed25519::create_keypair(keys.secret_key()));
}
address_bundle account::get_publish_addr(void) const
{
	auto keys=master_.slip10_key_from_path(publish_path);
	const auto edkeys=qed25519::create_keypair(keys.secret_key());
	return address_bundle(edkeys);
}

qblocks::signature address_bundle::sign(const QByteArray & message)const
{
	return qblocks::signature(qed25519::sign(key_pair,message));
}
