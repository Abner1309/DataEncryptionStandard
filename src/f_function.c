#include <stdio.h>
#include <stdlib.h>
#include "f_function.h"
#include "utilities.h"

int* expansion(int* right_half) {
    // Expansion Index Matrix:
    int auxiliary[8][6] = {
        {32, 1, 2, 3, 4, 5},
        {4, 5, 6, 7, 8, 9},
        {8, 9, 10, 11, 12, 13},
        {12, 13, 14, 15, 16, 17},
        {16, 17, 18, 19, 20, 21},
        {20, 21, 22, 23, 24, 25},
        {24, 25, 26, 27, 28, 29},
        {28, 29, 30, 31, 32, 1},
    };

    // Memory Allocation - New Right Half:
    int* new_right_half = (int*) malloc(sizeof(int) * 48);
    if (new_right_half == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Right Half:
    for (int i = 0, j = 0, k = 0; i < 48; i++) {
        if (k > 5) {
            j++;
            k = 0;
        }
        new_right_half[i] = right_half[auxiliary[j][k] - 1];
        k++;
    }

    // Free Resources:
    free_vector(right_half);

    return new_right_half;
}

int* operation_xor(int* right_half, int* key) {
    // Memory Allocation - New Right Half:
    int* new_right_half = (int*) malloc(sizeof(int) * 48);
    if (new_right_half == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Right Half:
    for (int i = 0; i < 48; i++) {
        if (right_half[i] == key[i]) {
            new_right_half[i] = 0;
        }
        else {
            new_right_half[i] = 1;
        }
    }

    // Free Resources:
    free_vector(right_half);
    free_vector(key);

    return new_right_half;
}
