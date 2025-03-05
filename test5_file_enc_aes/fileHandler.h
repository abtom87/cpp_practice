
#pragma once

#include "aescrypto.h"
#include <fstream>
#include <iostream>

class FileHandler {

private:
  std::string mFileContents;
  std::uint32_t mFileSize;
  std::uint32_t mNumIterations;
  std::uint32_t mNumRemBytes;
  FileCryptoAES mAESCryptoOps;
  std::ifstream mFileInput;
  std::ofstream mFileOutput;

  void get_file_content(const std::string &file_path);

public:
  FileHandler(FileCryptoAES &objAES) : mAESCryptoOps(objAES) {}
  void encrypt_and_write_output(const std::string &inp_file_path,
                                const std::string &out_file_path);
  void decrypt_and_write_output(const std::string &encrypted_file_path,
                                const std::string &decrypted_file_path);
};