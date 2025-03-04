#include "aescrypto.h"

int main() {
  std::string file_password;
  std::cout << "Enter password: ";
  std::cin >> file_password;

  std::string hashedPass = "";
  FileCryptoAES objAESCrypto(file_password);

  // Initialisation vector
  objAESCrypto.fill_iv_buffer();

  // Generate Key from entered password
  objAESCrypto.generate_key_from_pass();

  std::cout << "Encrypting..." << std::endl;
  objAESCrypto.encrypt();

  std::cout << "Decrypting..." << std::endl;
  objAESCrypto.decrypt();

  std::cout << "Decrypted output..." << std::endl;
  objAESCrypto.print_decrypted_buff();

  return 0;
}