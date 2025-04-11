## Description
A simple command line based tool to encrypt and decrypt any file using AES CBC algorithm.

## Compile
```
mkdir build
cd build
cmake ..
make
```

## Usage
To encrypt a file, e stands for encrypt. Enter a password, SHA256 will be applied over it to encrypt the file.
```
./file_enc_aes -e FiletoEncrypt EncryptedFileName
```

To decrypt an already encrypted file, , d stands for decrypt. Enter the same password you used to encrypt.
```
./file_enc_aes -d EncryptedFileName DecryptedFileName
```
