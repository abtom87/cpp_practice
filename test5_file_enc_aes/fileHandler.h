
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
  std::ifstream mFileToEncrypt;
  std::ofstream mFileToDecrypt;

  std::string get_file_content(const std::string &file_path);

public:
  FileHandler(const std::string &file_path, FileCryptoAES &objAES)
      : mAESCryptoOps(objAES) {

    mFileToEncrypt.open(file_path);
    mFileToDecrypt.open(file_path + "out");

    if (false == mFileToEncrypt.good()) {
      std::cerr << "Error opening " << file_path << '\n';
    } else {

      // Read the whole chunk
      std::string fileContents((std::istreambuf_iterator<char>(mFileToEncrypt)),
                               std::istreambuf_iterator<char>());

      mFileContents = fileContents;
      mFileSize = mFileContents.length();

      mNumIterations = mFileSize / mAESCryptoOps.get_inp_buffer_size();

      mNumRemBytes = mFileSize % mAESCryptoOps.get_inp_buffer_size();
    }
  }
  void encrypt_and_write_output();
};