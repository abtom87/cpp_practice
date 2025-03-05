
#include "fileHandler.h"

void FileHandler::encrypt_and_write_output() {
  std::uint32_t begin_index, end_index;
  std::cout << "Total Bytes: " << mFileSize << '\n';
  std::cout << "Total iter: " << mNumIterations << '\n';

  // Encryted buffer length
  std::uint8_t buff_len = mAESCryptoOps.get_inp_buffer_size();

  for (std::uint32_t i = 0; i < mNumIterations; i++) {
    begin_index = i * buff_len;     // 0, 64, 128
    end_index = (i + 1) * buff_len; // 63, 127, 191

    std::vector<std::uint8_t> temp_vec(mFileContents.begin() + begin_index,
                                       mFileContents.begin() + end_index);

    // Pass this temp_vec to encrypt function
    mAESCryptoOps.encrypt(temp_vec);

    // Get the output vector and append it to a file
    std::vector<std::uint8_t> enc_vec_temp =
        mAESCryptoOps.get_encrypted_vector();
    mFileToDecrypt.write(reinterpret_cast<const char *>(enc_vec_temp.data()),
                         enc_vec_temp.size());
  }

  std::cout << "REM BYTES: " << mNumRemBytes << '\n';
  if (mNumRemBytes > 0) {
    begin_index = mNumIterations * buff_len;
    end_index = begin_index + mNumRemBytes;

    std::vector<std::uint8_t> temp_vec_rem(mFileContents.begin() + begin_index,
                                           mFileContents.begin() + end_index);

    for (const char &c : temp_vec_rem)
      std::cout << c;

    // Pass this temp_vec to encrypt function
    mAESCryptoOps.encrypt(temp_vec_rem);

    // Get the output vector and append it to a file
    std::vector<std::uint8_t> enc_vec_temp =
        mAESCryptoOps.get_encrypted_vector();

    mFileToDecrypt.write(reinterpret_cast<const char *>(enc_vec_temp.data()),
                         enc_vec_temp.size());
  }

  mFileToEncrypt.close();
  mFileToDecrypt.close();
}