#include "aescrypto.h"
#include <iomanip>
#include <memory>
#include <openssl/evp.h>
#include <openssl/rand.h>
void FileCryptoAES::hex_to_bytes(const std::string &hex) {

  unsigned char bytes[kLenKey];

  for (size_t i = 0; i < kLenKey; i++) {
    std::sscanf(hex.c_str() + (i * 2), "%2hhx", &bytes[i]);
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
  auto const mdCtx = std::unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX *)>(
      EVP_MD_CTX_new(), &EVP_MD_CTX_free);

  unsigned char mdVal[EVP_MAX_MD_SIZE];
  unsigned int mdLen;

  EVP_DigestInit_ex(mdCtx.get(), EVP_sha256(), NULL);
  EVP_DigestUpdate(mdCtx.get(), get_file_password().c_str(),
                   get_file_password().length());
  EVP_DigestFinal_ex(mdCtx.get(), mdVal, &mdLen);

  std::stringstream ss;
  for (unsigned int i = 0; i < mdLen; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<std::uint16_t>(mdVal[i]);
  }
  hashedpass = ss.str();

  set_hashed_pass(hashedpass);
  return true;
}

void FileCryptoAES::generate_key_from_pass() {
  // generate SHA256 hash from entered pass
  calc_sha256();
  std::cout << "SHA256 password: " << get_hashed_pass() << std::endl;

  hex_to_bytes(get_hashed_pass());
}

void FileCryptoAES::encrypt(const std::vector<std::uint8_t> &inp_vec) {
  unsigned char *pKeyBuff = mKeyBuffer.data();
  unsigned char *pIVBuff = mInitialisationVector.data();
  unsigned char *pOutBuff = mOutputBuffer.data();

  const unsigned char *pInpBuff = inp_vec.data();

  auto const enc_Ctx =
      std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)>(
          EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);

  EVP_EncryptInit_ex(enc_Ctx.get(), EVP_aes_256_cbc(), NULL, pKeyBuff, pIVBuff);

  EVP_EncryptUpdate(enc_Ctx.get(), pOutBuff, &mOutLen, pInpBuff,
                    inp_vec.size());

  EVP_EncryptFinal_ex(enc_Ctx.get(), pOutBuff + mOutLen, &mFinalOutLen);

  // Correctly set the final output length
  mOutLen += mFinalOutLen;

  // Out len is needed to decrypt correctly
  mOutLenVect.push_back(mOutLen);
}

void FileCryptoAES::decrypt(const std::vector<std::uint8_t> &inp_vec,
                            std::uint8_t &final_out_len) {

  unsigned char *pKeyBuff = mKeyBuffer.data();
  unsigned char *pIVBuff = mInitialisationVector.data();
  const unsigned char *pEncryptedBuff = (const unsigned char *)inp_vec.data();

  unsigned char *pDecBuff = mDecryptedBuffer.data();

  auto const dec_ctx =
      std::unique_ptr<EVP_CIPHER_CTX, void (*)(EVP_CIPHER_CTX *)>(
          EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
  EVP_DecryptInit_ex(dec_ctx.get(), EVP_aes_256_cbc(), NULL, pKeyBuff, pIVBuff);
  EVP_DecryptUpdate(dec_ctx.get(), pDecBuff, &mDecLen, pEncryptedBuff,
                    final_out_len);
  EVP_DecryptFinal_ex(dec_ctx.get(), pDecBuff + mDecLen, &mFinalDecLen);

  mDecLen += mFinalDecLen;
  mDecryptedBuffer.resize(mDecLen);
}

void FileCryptoAES::print_decrypted_buff() {

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
