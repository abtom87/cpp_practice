
#include "fileHandler.h"
#include <algorithm>

void FileHandler::get_file_content(const std::string &file_path) {

  mFileInput.open(file_path, std::ios::app | std::ios::binary);

  if (false == mFileInput.good()) {
    std::cerr << "Error opening " << file_path << '\n';
    return;
  } else {

    // Read the whole chunk
    std::string fileContents((std::istreambuf_iterator<char>(mFileInput)),
                             std::istreambuf_iterator<char>());

    mFileContents = fileContents;
    mFileSize = mFileContents.length();

    mNumIterations = mFileSize / mAESCryptoOps.get_inp_buffer_size();

    mNumRemBytes = mFileSize % mAESCryptoOps.get_inp_buffer_size();
  }
}
void FileHandler::encrypt_and_write_output(const std::string &inp_file_path,
                                           const std::string &out_file_path) {

  get_file_content(inp_file_path);
  mFileOutput.open(out_file_path);

  std::uint32_t begin_index, end_index;

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
    mFileOutput.write(reinterpret_cast<const char *>(enc_vec_temp.data()),
                      enc_vec_temp.size());
  }

  if (mNumRemBytes > 0) {
    begin_index = mNumIterations * buff_len;
    end_index = begin_index + mNumRemBytes;

    std::vector<std::uint8_t> temp_vec_rem(mFileContents.begin() + begin_index,
                                           mFileContents.begin() + end_index);

    // Pass this temp_vec to encrypt function
    mAESCryptoOps.encrypt(temp_vec_rem);

    // Get the output vector and append it to a file
    std::vector<std::uint8_t> enc_vec_temp =
        mAESCryptoOps.get_encrypted_vector();

    mFileOutput.write(reinterpret_cast<const char *>(enc_vec_temp.data()),
                      enc_vec_temp.size());
  }

  mFileInput.close();
  mFileOutput.close();
}

void FileHandler::decrypt_and_write_output(
    const std::string &encrypted_file_path,
    const std::string &decrypted_file_path) {

  get_file_content(encrypted_file_path);
  mFileOutput.open(decrypted_file_path);

  std::uint32_t begin_index = 0;
  std::uint32_t i = 0;
  std::vector<std::uint8_t> OutLenVect = mAESCryptoOps.get_outlen_vect();
  while (begin_index < mFileSize) {
    std::uint32_t chunk_size =
        std::min((std::uint32_t)mAESCryptoOps.get_out_buffer_size(),
                 (std::uint32_t)(mFileSize - begin_index));

    std::vector<std::uint8_t> temp_vec(mFileContents.begin() + begin_index,
                                       mFileContents.begin() + begin_index +
                                           chunk_size);

    mAESCryptoOps.decrypt(temp_vec, OutLenVect.at(i));
    i++;

    std::vector<std::uint8_t> dec_vec_temp =
        mAESCryptoOps.get_decrypted_vector();
    mFileOutput.write(reinterpret_cast<const char *>(dec_vec_temp.data()),
                      dec_vec_temp.size());

    begin_index += chunk_size;
  }

  mFileInput.close();
  mFileOutput.close();
}
