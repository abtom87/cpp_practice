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

  std::vector<std::uint8_t> mInputBuffer;
  std::vector<std::uint8_t> mOutputBuffer;
  std::vector<std::uint8_t> mDecryptedBuffer;

  int outLen = 0, finalOutLen = 0;

  FileCryptoAES()
      : mKeyBuffer(kLenKey), mInitialisationVector(kIVLen),
        mOutputBuffer(kBuffSize), mDecryptedBuffer(kBuffSize),
        mInputBuffer(kBuffSize) {}

  bool calc_sha256(std::string inp_pass, std::string &hashed_pass);
};