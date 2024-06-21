#include "Encryption.h"

static const unsigned int AES_KEY_LENGTH = 256;
static const unsigned int AES_BLOCK_SIZE = 16;

std::string Encryption::encrypt(const std::string& plainText, const std::string& key)
{
    // Generate initialization vector
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE))
    {
        std::cerr << ("Failed to generate IV");
    }

    // Prepare encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << ("Failed to create encryption context");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
        reinterpret_cast<const unsigned char*>(key.data()), iv) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to initialize encryption");
    }

    // Encrypt the data
    std::vector<unsigned char> cipherText(plainText.size() + AES_BLOCK_SIZE);
    int len = 0, cipherTextLen = 0;

    if (EVP_EncryptUpdate(ctx, cipherText.data(), &len,
        reinterpret_cast<const unsigned char*>(plainText.data()), plainText.size()) != 1) 
    {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to encrypt data");
    }
    cipherTextLen = len;

    if (EVP_EncryptFinal_ex(ctx, cipherText.data() + len, &len) != 1)
    {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to finalize encryption");
    }

    cipherTextLen += len;
    cipherText.resize(cipherTextLen);

    EVP_CIPHER_CTX_free(ctx);

    // Prepend IV to ciphertext
    std::string result(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE);
    result.append(reinterpret_cast<char*>(cipherText.data()), cipherText.size());

    return result;
}

std::string Encryption::decrypt(const std::string& cipherText, const std::string& key)
{
    // Extract the IV from the ciphertext
    if (cipherText.size() < AES_BLOCK_SIZE) {
        std::cerr << ("Invalid ciphertext");
    }

    unsigned char iv[AES_BLOCK_SIZE];
    std::copy(cipherText.begin(), cipherText.begin() + AES_BLOCK_SIZE, iv);

    // Prepare the decryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        std::cerr << ("Failed to create decryption context");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
        reinterpret_cast<const unsigned char*>(key.data()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to initialize decryption");
    }

    // Decrypt the data
    std::vector<unsigned char> plainText(cipherText.size() - AES_BLOCK_SIZE + AES_BLOCK_SIZE);
    int len = 0, plainTextLen = 0;

    if (EVP_DecryptUpdate(ctx, plainText.data(), &len,
        reinterpret_cast<const unsigned char*>(cipherText.data()) + AES_BLOCK_SIZE, cipherText.size() - AES_BLOCK_SIZE) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to decrypt data");
    }
    plainTextLen = len;

    if (EVP_DecryptFinal_ex(ctx, plainText.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        std::cerr << ("Failed to finalize decryption");
    } 
    plainTextLen += len;
    plainText.resize(plainTextLen);

    EVP_CIPHER_CTX_free(ctx);

    return std::string(reinterpret_cast<char*>(plainText.data()), plainText.size());
}

std::string Encryption::generateKeyHash(const std::string& key)
{
    unsigned char hash[EVP_MAX_KEY_LENGTH];
    EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(), nullptr,
        reinterpret_cast<const unsigned char*>(key.c_str()), key.length(), 1, hash, nullptr);
    return std::string(reinterpret_cast<char*>(hash), EVP_MAX_KEY_LENGTH);
}