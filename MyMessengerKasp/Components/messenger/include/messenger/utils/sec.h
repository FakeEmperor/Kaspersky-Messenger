/*

#pragma once
#include <openssl/aes.h>
#include <memory>
#include <string>
#include <openssl/sha.h>
#include <array>

namespace utils
{
	namespace sec
	{
		typedef unsigned char byte;

		typedef std::array<byte, SHA256_DIGEST_LENGTH> Key;
		typedef std::array<byte, SHA256_DIGEST_LENGTH> Salt;
		typedef std::array<byte, SHA256_DIGEST_LENGTH> Hash;


		Hash HashString(const std::string str);

		Key MakeKey(const std::string password);

		std::string Encrypt(const std::string& in, const Key& k);

		std::string Decrypt(const std::string& in, const Key& k);

		AES_KEY MakeEncryptionKey(const Key key);

		AES_KEY MakeDecryptionKey(const Key& key);
	}
}
*/