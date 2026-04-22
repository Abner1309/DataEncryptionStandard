#ifndef DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
#define DATAENCRYPTIONSTANDARD_MAIN_STREAM_H

int* initial_permutation(int* message);
int** divide_message(int* message);
int* operation_xor_feistel(int* message1, int* message2);
int* feistel_helper(int* message);
int** feistel_scheme(int** messages, int** sixteen_keys);

#endif //DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
