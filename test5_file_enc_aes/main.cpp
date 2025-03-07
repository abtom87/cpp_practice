#include "aescrypto.h"

#include "fileHandler.h"
#include <cstring>
#include <unistd.h>

int main(int argc, char **argv) {

#if 0
  if (argc == 1 || argc > 4) {

    std::cout << argc;
    std::cout
        << "No Extra Command Line Argument Passed Other Than Program Name\n";
    std::cout << "USAGE: prog -e InputfileName OutPutFileName OR prog -d "
                 "Encryptedfilename "
                 " OutputFileName\n ";
    exit(1);
  }
#endif

  std::string file_password;
  std::cout << "Enter password: ";
  std::cin >> file_password;
  FileCryptoAES objAESCrypto(file_password);

  FileHandler objFHandler(objAESCrypto);
  /*
    if (std::strcmp(argv[1], "-e") == 0) {
      const std::string enc_inp_file_name = argv[2];
      const std::string enc_out_file_name = argv[3];
      objFHandler.encrypt_and_write_output(enc_inp_file_name,
    enc_out_file_name); } else if ((std::strcmp(argv[1], "-d") == 0)) { const
    std::string dec_inp_file_name = argv[2]; const std::string dec_out_file_name
    = argv[3]; objFHandler.decrypt_and_write_output(dec_inp_file_name,
    dec_out_file_name); printf("%s", argv[2]);
    }
  */
  const std::string enc_inp_file_name = "file";
  const std::string enc_out_file_name = "file_enc";

  const std::string dec_inp_file_name = "file_enc";
  const std::string dec_out_file_name = "file_out";

  objFHandler.encrypt_and_write_output(enc_inp_file_name, enc_out_file_name);
  objFHandler.decrypt_and_write_output(dec_inp_file_name, dec_out_file_name);

  return 0;
}