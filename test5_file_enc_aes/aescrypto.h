#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdbool.h>
#include <vector>

class FileCryptoAES {

private:
  static constexpr uint8_t kBuffSize = 64;
  static constexpr uint8_t kLenKey = 32;
  static constexpr uint8_t kIVLen = 16;

  std::string mfilePassword;
  std::string mhashedPassword;

  std::vector<std::uint8_t> mKeyBuffer;
  std::vector<std::uint8_t> mInitialisationVector;

  std::vector<std::uint8_t> mOutputBuffer;
  std::vector<std::uint8_t> mDecryptedBuffer;
  std::vector<std::uint8_t> mInputBuffer;

  int mOutLen, mFinalOutLen = 0;
  int mDecLen, mFinalDecLen;

  // Convert the first 32 hex characters (64 chars) into 32 raw bytes for AES
  // key
  void hexToBytes(const std::string &hex);

  const std::string getFilePassword() { return mfilePassword; }

  bool calc_sha256();
  void set_hashed_pass(std::string &hashed_pass) {

    mhashedPassword = hashed_pass;
  }

  std::string get_hashed_pass() { return mhashedPassword; }

public:
  FileCryptoAES(std::string password)
      : mKeyBuffer(kLenKey), mInitialisationVector(kIVLen),
        mOutputBuffer(kBuffSize), mDecryptedBuffer(kBuffSize),
        mInputBuffer(kBuffSize) {

    mfilePassword = password;
  }

  void fill_iv_buffer();
  void generate_key_from_pass();
  void encrypt(const std::vector<std::uint8_t> &inp_vec);
  void decrypt();
  void print_decrypted_buff();
};