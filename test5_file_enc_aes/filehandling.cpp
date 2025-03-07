
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
  }
}

void FileHandler::process_file(const std::string &input_path,
                               const std::string &output_path, bool encrypt) {
  get_file_content(input_path);
  mFileOutput.open(output_path);

  std::uint32_t begin_index = 0;
  std::uint32_t i = 0;
  std::vector<std::uint8_t> out_len_vect = mAESCryptoOps.get_outlen_vect();
  std::uint8_t buffer_size = encrypt ? mAESCryptoOps.get_inp_buffer_size()
                                     : mAESCryptoOps.get_out_buffer_size();

  while (begin_index < mFileSize) {
    std::uint32_t chunk_size = std::min(static_cast<std::uint32_t>(buffer_size),
                                        mFileSize - begin_index);

    std::vector<std::uint8_t> temp_vec(mFileContents.begin() + begin_index,
                                       mFileContents.begin() + begin_index +
                                           chunk_size);

    std::vector<std::uint8_t> processed_vec;

    if (encrypt) {
      mAESCryptoOps.encrypt(temp_vec);
      processed_vec = mAESCryptoOps.get_encrypted_vector();
    } else {
      mAESCryptoOps.decrypt(temp_vec, out_len_vect.at(i));
      processed_vec = mAESCryptoOps.get_decrypted_vector();
      i++;
    }

    mFileOutput.write(reinterpret_cast<const char *>(processed_vec.data()),
                      processed_vec.size());

    begin_index += chunk_size;
  }

  mFileInput.close();
  mFileOutput.close();
}

void FileHandler::encrypt_and_write_output(const std::string &inp_file_path,
                                           const std::string &out_file_path) {

  process_file(inp_file_path, out_file_path, true);
}

void FileHandler::decrypt_and_write_output(
    const std::string &encrypted_file_path,
    const std::string &decrypted_file_path) {
  process_file(encrypted_file_path, decrypted_file_path, false);
}