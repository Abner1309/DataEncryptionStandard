#include <stdio.h>
#include <stdlib.h>
#include "key_schedule.h"
#include "utilities.h"

int** permuted_choice_1(int* original_key) {
    // Useful Variables:
    int rows = 8, columns = 7, half_key = 28;

    // Memory Allocation: Left Part (C0) + Right Part (D0):
    int** two_keys = (int**) malloc(sizeof(int*) * 2);
    if (two_keys == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 2; i++) {
        two_keys[i] = (int*) malloc(sizeof(int) * half_key);
        if (two_keys[i] == NULL) {
            fprintf(stderr, "Error: Memory could not be allocated.\n");
            exit(EXIT_FAILURE);
        }
    }
    // Memory Allocation: Auxiliary Matrix:
    int** matrix = (int**) malloc(sizeof(int*) * rows);
    if (matrix == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * columns);
        if (matrix[i] == NULL) {
            fprintf(stderr, "Error: Memory could not be allocated.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fill Auxiliary Matrix:
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = original_key[i * columns + j];
        }
    }

    // Fill C0 and D0:
    for (int i = 0; i < 2; i++) {
        // Fill Left Part (C0):
        for (int j = 0, k = rows - 1, l = 0; j < half_key; j++, k--) {
            if (k < 0) {
                k = rows - 1;
                l++;
            }
            two_keys[i][j] = matrix[k][l];
        }
        i++;
        // Fill Right Part (D0):
        for (int j = 0, k = rows - 1, l = columns - 1, count = 1; j < half_key; j++, k--) {
            if (k < 0) {
                k = rows - 1;
                l--;
                count++;
                if (count == 4) {
                    k = rows - 5;
                }
            }
            two_keys[i][j] = matrix[k][l];
        }
    }

    // Free Allocated Memory For Auxiliary Matrix:
    free_matrix(matrix, rows);

    return two_keys;
}

int* circular_left_shift(int* half_key, int jumps) {
    int mem;
    // Memory Allocation:
    int* new_key = (int*) malloc(sizeof(int) * 28);
    if (new_key == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(EXIT_FAILURE);
    }

    // Circular Left Shift
    for (int i = 0; i < jumps; i++) {
        mem = half_key[0];
        for (int j = 0, k = 1; k < 28; j++, k++) {
            new_key[j] = half_key[k];
        }
        new_key[27] = mem;
    }

    // Free Allocated Memory:
    free_vector(half_key);

    return new_key;
}

int* permuted_choice_2(int* left_half_key, int* right_half_key) {
    // Permuted Choice Index Table:
    int auxiliary_matrix[6][8] = {
        {14, 17, 11, 24, 1, 5, 3, 28},
        {15, 6, 21, 10, 23, 19, 12, 4},
        {26, 8, 16, 7, 27, 20, 13, 2},
        {41, 52, 31, 37, 47, 55, 30, 40},
        {51, 45, 33, 48, 44, 49, 39, 56},
        {34, 53, 46, 42, 50, 36, 29, 32}
    };

    // Memory Allocation:
    int* key_pc2 = (int*) malloc(sizeof(int) * 48);
    if (key_pc2 == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(EXIT_FAILURE);
    }

    // Concatenation of the Left and Right:
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (auxiliary_matrix[i][j] - 1 < 28) {
                key_pc2[i * 8 + j] = left_half_key[auxiliary_matrix[i][j] - 1];
            }
            else {
                key_pc2[i * 8 + j] = right_half_key[auxiliary_matrix[i][j] - 29];
            }
        }
    }

    return key_pc2;
}

int** key_schedule(int* original_key) {
    // Memory Allocation: Sixteen Subkeys
    int** sixteen_subkeys = (int**) malloc(sizeof(int*) * 16);
    if (sixteen_subkeys == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 16; i++) {
        sixteen_subkeys[i] = (int*) malloc(sizeof(int) * 48);
        if (sixteen_subkeys[i] == NULL) {
            fprintf(stderr, "Error: Memory could not be allocated.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Call Permuted Choice 1:
    int** left_right_parts = permuted_choice_1(original_key);

    // Generation of Sixteen SubKeys:
    for (int i = 0, j = 0; i < 16; i++) {
        if (i == 0 || i == 1 || i == 8 || i == 15) {
            j = 1;
        }
        else {
            j = 2;
        }

        // Call Circular Left Shift:
        int* left_shift = circular_left_shift(left_right_parts[0], j);
        int* right_shift = circular_left_shift(left_right_parts[1], j);
        left_right_parts[0] = left_shift;
        left_right_parts[1] = right_shift;

        // Call Permuted Choice 2:
        sixteen_subkeys[i] = permuted_choice_2(left_shift, right_shift);
    }

    free_matrix(left_right_parts, 2);

    return sixteen_subkeys;
}
