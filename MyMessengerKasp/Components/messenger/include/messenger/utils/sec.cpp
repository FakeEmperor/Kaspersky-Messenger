/*#include "sec.h"

utils::sec::Hash utils::sec::HashString(const std::string str)
{
	Hash hash;
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(const_cast<byte*>(hash.data()), &sha256);
	return hash;
}

AES_KEY utils::sec::MakeEncryptionKey(const Key k)
{
	AES_KEY key;
	AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(&k), CHAR_BIT * 16, &key);
	return key;
}

AES_KEY utils::sec::MakeDecryptionKey(const Key& k)
{
	AES_KEY key;
	AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(&k), CHAR_BIT * 16, &key);
	return key;
}

utils::sec::Key utils::sec::MakeKey(const std::string password)
{
	const std::string salt = "LOLOLOLO";
	return static_cast<Key>(HashString(password + salt));
}

std::string utils::sec::Encrypt(const std::string& in, const Key& k)
{
	AES_KEY key = utils::sec::MakeEncryptionKey(k);
	std::string out;
	std::shared_ptr<byte> buf(new byte[in.size()]);
	AES_set_encrypt_key(reinterpret_cast<const unsigned char*>(&k), CHAR_BIT * 16, &key);
	AES_encrypt(reinterpret_cast<const unsigned char*>(in.data()), buf.get(), &key );
	out.assign(buf.get(), buf.get()+in.size());
	return out;
}

std::string utils::sec::Decrypt(const std::string& in, const Key& k)
{
	AES_KEY key;
	std::string out;
	std::shared_ptr<byte> buf(new byte[in.size()]);
	AES_set_decrypt_key(reinterpret_cast<const unsigned char*>(&k), CHAR_BIT * 16, &key);
	AES_decrypt(reinterpret_cast<const unsigned char*>(in.data()), buf.get(), &key);
	out.assign(buf.get(), buf.get() + in.size());
	return out;
}
*/