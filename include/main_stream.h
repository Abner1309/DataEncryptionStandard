#ifndef DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
#define DATAENCRYPTIONSTANDARD_MAIN_STREAM_H

int* initial_permutation(int* message);
int** divide_message(int* message);
int** feistel_scheme(int** messages);

#endif //DATAENCRYPTIONSTANDARD_MAIN_STREAM_H
