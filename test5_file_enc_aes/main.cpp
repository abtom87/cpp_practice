#include "aescrypto.h"

int main() {
  std::string file_password;
  std::cout << "Enter password: ";
  std::cin >> file_password;

  FileCryptoAES objAESCrypto(file_password);

  unsigned char inBuffer[] = "HELLO EQ 123566 ABCDEF 1211 34567";

  // Initialize a vector from the array
  std::vector<std::uint8_t> input_vec(std::begin(inBuffer), std::end(inBuffer));
  std::cout << "Inp data: " << input_vec.data() << std::endl;

  std::cout << "Encrypting..." << std::endl;
  objAESCrypto.encrypt(input_vec);

  std::cout << "Decrypting..." << std::endl;
  objAESCrypto.decrypt();

  std::cout << "Decrypted output..." << std::endl;
  objAESCrypto.print_decrypted_buff();

  return 0;
}