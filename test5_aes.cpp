#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>

std::string sha256(std::string password) {
  EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
  unsigned char mdVal[EVP_MAX_MD_SIZE];
  unsigned int mdLen;
  EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL);
  EVP_DigestUpdate(mdCtx, password.c_str(), password.length());
  EVP_DigestFinal_ex(mdCtx, mdVal, &mdLen);
  EVP_MD_CTX_free(mdCtx);

  std::stringstream ss;
  for (unsigned int i = 0; i < mdLen; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)mdVal[i];
  }
  return ss.str();
}

// Convert the first 32 hex characters (64 chars) into 32 raw bytes for AES key
void hexToBytes(const std::string &hex, unsigned char *bytes, size_t bytesLen) {
  for (size_t i = 0; i < bytesLen; i++) {
    sscanf(hex.c_str() + (i * 2), "%2hhx", &bytes[i]);
  }
}

unsigned char key[32] = {0};
unsigned char iv[16] = {0};

unsigned char outBuffer[64] = {0};
int outLen = 0, finalOutLen = 0;

void encrypt() {
  unsigned char inBuffer[] = "HELLO PLATTRYW";
  EVP_CIPHER_CTX *enc_Ctx = EVP_CIPHER_CTX_new();

  EVP_EncryptInit_ex(enc_Ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_EncryptUpdate(enc_Ctx, outBuffer, &outLen, inBuffer,
                    strlen((char *)inBuffer));
  EVP_EncryptFinal_ex(enc_Ctx, outBuffer + outLen, &finalOutLen);

  outLen += finalOutLen; // Ensure final output length is correct
  EVP_CIPHER_CTX_free(enc_Ctx);
}

unsigned char decryptedBuffer[64] = {0};
int decLen = 0, finalDecLen = 0;

void decrypt() {
  EVP_CIPHER_CTX *dec_ctx = EVP_CIPHER_CTX_new();

  EVP_DecryptInit_ex(dec_ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_DecryptUpdate(dec_ctx, decryptedBuffer, &decLen, outBuffer, outLen);
  EVP_DecryptFinal_ex(dec_ctx, decryptedBuffer + decLen, &finalDecLen);

  decLen += finalDecLen;
  decryptedBuffer[decLen] = '\0'; // Ensure null-termination

  EVP_CIPHER_CTX_free(dec_ctx);
}
namespace fs = std::filesystem;
std::string readFile(std::filesystem::path path) {
  // Open the stream to 'lock' the file.
  std::ifstream f(path, std::ios::in | std::ios::binary);

  // Obtain the size of the file.
  const auto sz = fs::file_size(path);

  // Create a buffer.
  std::string result(sz, '\0');

  // Read the whole file into the buffer.
  f.read(result.data(), sz);

  return result;
}
int main() {
  RAND_bytes(iv, sizeof(iv)); // Generate a random IV

  std::string file_password;
  std::cout << "Enter password: ";
  std::cin >> file_password;

  std::string hashed = sha256(file_password);
  std::cout << "SHA-256 Hash: " << hashed << std::endl;

  // Convert first 32 bytes of hex string to raw bytes for AES key
  hexToBytes(hashed, key, sizeof(key));

  std::cout << "Encrypting..." << std::endl;
  encrypt();

  std::cout << "Decrypting..." << std::endl;
  decrypt();

  std::cout << "Decrypted Output: " << decryptedBuffer << std::endl;

  return 0;
}
