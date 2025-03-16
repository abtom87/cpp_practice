
#include "fileHandler.h"
#include <algorithm>
#include <sstream>

void FileHandler::get_file_content(const std::string &file_path) {

  mFileInput.open(file_path);

  if (false == mFileInput.good()) {
    std::cerr << "Error opening " << file_path << ". Exiting..." << '\n';
    std::exit(EXIT_FAILURE);
  } else {
    mFileInput.open(file_path, std::ios::app | std::ios::binary);
    // Read the whole chunk
    std::string fileContents((std::istreambuf_iterator<char>(mFileInput)),
                             std::istreambuf_iterator<char>());

    mFileContents = fileContents;
    mFileSize = mFileContents.length();
  }
  mFileInput.close();
}

void FileHandler::process_file(const std::string &input_path,
                               const std::string &output_path, bool encrypt) {
  get_file_content(input_path);
  mFileOutput.open(output_path);

  std::uint32_t begin_index = 0;
  std::uint32_t i = 0;

  std::uint16_t buffer_size = encrypt ? mAESCryptoOps.get_inp_buffer_size()
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
      mAESCryptoOps.decrypt(temp_vec, mAESCryptoOps.get_outlen_vect().at(i));
      processed_vec = mAESCryptoOps.get_decrypted_vector();
      i++;
    }

    mFileOutput.write(reinterpret_cast<const char *>(processed_vec.data()),
                      processed_vec.size());

    begin_index += chunk_size;
  }

  mFileOutput.close();
}

void FileHandler::encrypt_and_write_output(const std::string &inp_file_path,
                                           const std::string &out_file_path) {

  process_file(inp_file_path, out_file_path, true);

  std::ofstream encrypted_file(out_file_path, std::ios::app | std::ios::binary);
  encrypted_file << "DEADBEEF"
                 << ",";
  for (auto &elem : mAESCryptoOps.get_outlen_vect()) {
    encrypted_file << static_cast<uint16_t>(elem);
    encrypted_file << ",";
  }

  encrypted_file << "DEADBEEF";
  encrypted_file.close();
}

void FileHandler::decrypt_and_write_output(
    const std::string &encrypted_file_path,
    const std::string &decrypted_file_path) {

  get_file_content(encrypted_file_path);

  std::string initial_pattern = "DEADBEEF,";
  std::string last_pattern = ",DEADBEEF";

  auto first_occurrence =
      mFileContents.find(initial_pattern) + initial_pattern.length();
  auto last_occurrence =
      mFileContents.find(last_pattern) - last_pattern.length();
  std::string strNew = mFileContents.substr(first_occurrence, last_occurrence);
  // std::cout << strNew << "  " << '\n';

  std::stringstream ss(strNew);
  std::vector<uint16_t> vect;
  for (uint16_t i; ss >> i;) {
    vect.push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }
  mAESCryptoOps.set_outlen_vect(vect);

  // for (std::size_t i = 0; i < mAESCryptoOps.get_outlen_vect().size(); i++)
  // std::cout << mAESCryptoOps.get_outlen_vect().at(i) << std::endl;

  mFileContents.replace(first_occurrence - initial_pattern.length(),
                        last_occurrence + last_pattern.length(), "");

  // Reopen the encrypted file and put everything except outlen vector
  std::ofstream encrypted_file;
  encrypted_file.open(encrypted_file_path, std::ios::binary);
  encrypted_file << mFileContents;
  encrypted_file.close();

  process_file(encrypted_file_path, decrypted_file_path, false);
}