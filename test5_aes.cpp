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

unsigned char key[32] = {0};
unsigned char outBuffer[32] = {0};
int outLen = 0;
unsigned char iv[32] = {0};

void encrypt() {

  RAND_bytes(iv, sizeof(iv));

  unsigned char inBuffer[] = "HELLO PLATTRYW";
  EVP_CIPHER_CTX *enc_Ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(enc_Ctx, EVP_aes_256_cbc(), NULL, key, iv);
  EVP_EncryptUpdate(enc_Ctx, outBuffer, &outLen, inBuffer, sizeof(inBuffer));

  EVP_EncryptFinal_ex(enc_Ctx, outBuffer, &outLen);
}
unsigned char decryptedBuffer[32] = {0};
int decLen;
void decrypt() {
  EVP_CIPHER_CTX *dec_ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(dec_ctx, EVP_aes_256_cbc(), NULL, key, iv);

  EVP_DecryptUpdate(dec_ctx, decryptedBuffer, &decLen, outBuffer, outLen);

  EVP_DecryptFinal_ex(dec_ctx, decryptedBuffer, &decLen);
}

int main() {

  std::string file_password = "";

  std::cin >> file_password;
  std::string hashed = sha256(file_password);

  encrypt();
  decrypt();
  return 0;
}