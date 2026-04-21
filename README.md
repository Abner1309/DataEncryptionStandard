# Data Encryption Standard

## 📜 Overview:
This project aims to provide an implementation of the DES algorithm in the C programming language.
The DES algorithm was developed by IBM in the late 1970s and for a long time was considered the gold standard of cryptography; however, it is currently considered insecure. It is based on symmetric cryptography, meaning that the algorithm uses the same key to encrypt and decrypt a message. Furthermore, the algorithm operates on fixed-size blocks (64 bits).

## 💡 Basic Operation:
DES is a block cipher that operates under the Feistel Network scheme. It does not encrypt the text bit by bit (like stream ciphers), but rather in fixed chunks.
1. Block Size: 64 bits.
2. Key Size: 64 nominal bits, but only 56 bits are effectively used (8 bits are used for parity/verification).
3. Process: The algorithm subjects the data block to 16 rounds of identical transformations.
