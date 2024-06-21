#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>

class Encryption
{
public:
	static std::string encrypt(const std::string& plainText, const std::string& key);
	static std::string decrypt(const std::string& cipherText, const std::string& key);
	static std::string generateKeyHash(const std::string& key);
};

#endif ENCRYPTION_H