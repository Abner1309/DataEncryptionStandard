#ifndef DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
#define DATAENCRYPTIONSTANDARD_MAIN_STREAM_H

int* initial_permutation(int* message);
int** divide_message(int* message);
int* operation_xor_feistel(int* message1, int* message2);
int* feistel_helper(int* message);
int** feistel_scheme_encryption(int** messages, int** sixteen_keys);
int** feistel_scheme_decryption(int** messages, int** sixteen_keys);
int* join_message_des(int** messages);
int* join_message_des_inverted(int** messages);
int* final_permutation(int* message);

#endif //DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
