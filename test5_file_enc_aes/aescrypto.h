#pragma once

#include <iostream>
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
  void hex_to_bytes(const std::string &hex);

  const std::string get_file_password() { return mfilePassword; }

  bool calc_sha256();
  void set_hashed_pass(std::string &hashed_pass) {

    mhashedPassword = hashed_pass;
  }

  std::string get_hashed_pass() { return mhashedPassword; }
  void fill_iv_buffer();
  void generate_key_from_pass();

public:
  FileCryptoAES(std::string password)
      : mKeyBuffer(kLenKey), mInitialisationVector(kIVLen),
        mOutputBuffer(kBuffSize + kIVLen), mDecryptedBuffer(kBuffSize),
        mInputBuffer(kBuffSize) {

    mfilePassword = password;

    // Initialisation vector
    fill_iv_buffer();

    // Generate Key from entered password
    generate_key_from_pass();
  }

  std::uint8_t get_inp_buffer_size() { return kBuffSize; }
  std::vector<std::uint8_t> &get_encrypted_vector() { return mOutputBuffer; }

  void encrypt(const std::vector<std::uint8_t> &inp_vec);
  void decrypt();
  void print_decrypted_buff();
};