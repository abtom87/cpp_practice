#include "aescrypto.h"

void FileCryptoAES::hexToBytes(const std::string &hex) {

  unsigned char bytes[kLenKey];

  for (size_t i = 0; i < kLenKey; i++) {
    sscanf(hex.c_str() + (i * 2), "%2hhx", &bytes[i]);
  }

  // copy from unsigned char buffer to vector
  mKeyBuffer.assign(bytes, bytes + kLenKey);
  std::cout << "Key buffer:    ";
  for (int i = 0; i < kLenKey; i++)
    printf("%02X", mKeyBuffer[i]);
  printf("\n");
}

bool FileCryptoAES::calc_sha256() {

  std::string hashedpass;
  EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
  unsigned char mdVal[EVP_MAX_MD_SIZE];
  unsigned int mdLen;
  EVP_DigestInit_ex(mdCtx, EVP_sha256(), NULL);
  EVP_DigestUpdate(mdCtx, getFilePassword().c_str(),
                   getFilePassword().length());
  EVP_DigestFinal_ex(mdCtx, mdVal, &mdLen);
  EVP_MD_CTX_free(mdCtx);

  std::stringstream ss;
  for (unsigned int i = 0; i < mdLen; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)mdVal[i];
  }
  hashedpass = ss.str();

  set_hashed_pass(hashedpass);
  return true;
}

void FileCryptoAES::generate_key_from_pass() {
  calc_sha256();
  std::cout << "SHA256 password: " << get_hashed_pass() << std::endl;

  hexToBytes(get_hashed_pass());
}

void FileCryptoAES::encrypt(const std::vector<std::uint8_t> &inp_vec) {

  unsigned char *pKeyBuff = &mKeyBuffer[0];
  unsigned char *pIVBuff = &mInitialisationVector[0];
  unsigned char *pOutBuff = &mOutputBuffer[0];

  const unsigned char *pInpBuff =
      reinterpret_cast<const unsigned char *>(inp_vec.data());

  EVP_CIPHER_CTX *enc_Ctx = EVP_CIPHER_CTX_new();

  EVP_EncryptInit_ex(enc_Ctx, EVP_aes_256_cbc(), NULL, pKeyBuff, pIVBuff);
  EVP_EncryptUpdate(enc_Ctx, pOutBuff, &mOutLen, pInpBuff, inp_vec.size());
  EVP_EncryptFinal_ex(enc_Ctx, pOutBuff + mOutLen, &mFinalOutLen);

  mOutLen += mFinalOutLen; // Ensure final output length is correct
  EVP_CIPHER_CTX_free(enc_Ctx);
}

void FileCryptoAES::decrypt() {
  unsigned char *pKeyBuff = &mKeyBuffer[0];
  unsigned char *pIVBuff = &mInitialisationVector[0];
  unsigned char *pOutBuff = &mOutputBuffer[0];

  unsigned char *pDecBuff = &mDecryptedBuffer[0];

  EVP_CIPHER_CTX *dec_ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(dec_ctx, EVP_aes_256_cbc(), NULL, pKeyBuff, pIVBuff);
  EVP_DecryptUpdate(dec_ctx, pDecBuff, &mDecLen, pOutBuff, mOutLen);
  EVP_DecryptFinal_ex(dec_ctx, pDecBuff + mDecLen, &mFinalDecLen);

  mDecLen += mFinalDecLen;
  EVP_CIPHER_CTX_free(dec_ctx);
}

void FileCryptoAES::print_decrypted_buff() {

  std::cout << "Decrypted buffer: ";
  for (size_t i = 0; i < mDecryptedBuffer.size(); i++)
    printf("%c", mDecryptedBuffer[i]);
  printf("\n");
}

void FileCryptoAES::fill_iv_buffer() {
  unsigned char *pIVBuffer = &mInitialisationVector[0];

  RAND_bytes(pIVBuffer, kIVLen);
  std::cout << "IV: ";
  for (int i = 0; i < kIVLen; i++)
    printf("%X", mInitialisationVector[i]);
  printf("\n");
}
