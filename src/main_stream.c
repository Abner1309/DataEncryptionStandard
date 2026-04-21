#include <stdio.h>
#include <stdlib.h>
#include "main_stream.h"
#include "utilities.h"

int* initial_permutation(int* message) {
    // Initial Permutation Index Table:
    int auxiliary_matrix[8][8] = {
        {58, 50, 42, 34, 26, 18, 10, 2},
        {60, 52, 44, 36, 28, 20, 12, 4},
        {62, 54, 46, 38, 30, 22, 14, 6},
        {64, 56, 48, 40, 32, 24, 16, 8},
        {57, 49, 41, 33, 25, 17, 9, 1},
        {59, 51, 43, 35, 27, 19, 11, 3},
        {61, 53, 45, 37, 29, 21, 13, 5},
        {63, 55, 47, 39, 31, 23, 15, 7},
    };

    // Memory Allocation - New Message:
    int* new_message = (int*) malloc(sizeof(int) * 64);
    if (new_message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill Nex Message:
    for (int i = 0, j = 0, k = 0; i < 64; i++) {
        if (k > 7) {
            j++;
            k = 0;
        }
        new_message[i] = message[auxiliary_matrix[j][k] - 1];
        k++;
    }

    // Free Resources:
    free_vector(message);

    return new_message;
}

int** divide_message(int* message) {
    // Memory Allocation - Two Halfs:
    int** two_messages = (int**) malloc(sizeof(int*) * 2);
    if (two_messages == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 2; i++) {
        two_messages[i] = (int*) malloc(sizeof(int) * 32);
        if (two_messages[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Split Messages:
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 32; j++) {
            two_messages[i][j] = message[i * 32 + j];
        }
    }

    // Free Resources:
    free_vector(message);

    return two_messages;
}

int main() {
    return 0;
}