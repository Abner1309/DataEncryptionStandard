#ifndef DATAENCRYPTIONSTANDARD_KEYSCHEDULE_H
#define DATAENCRYPTIONSTANDARD_KEYSCHEDULE_H

int** permuted_choice_1(int* original_key);
int* circular_left_shift(int* half_key, int jumps);
int* permuted_choice_2(int* left_half_key, int* right_half_key);
int** key_schedule(int* original_key);

#endif //DATAENCRYPTIONSTANDARD_KEYSCHEDULE_H
