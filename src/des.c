#include <stdio.h>
#include <stdlib.h>
#include "f_function.h"
#include "key_schedule.h"
#include "des.h"
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

    // Fill New Message:
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
    // Memory Allocation - Two Halves:
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

int* operation_xor_feistel(int* message1, int* message2) {
    // Memory Allocation - New Message:
    int* new_message = (int*) malloc(sizeof(int) * 32);
    if (new_message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Message:
    for (int i = 0; i < 32; i++) {
        if (message1[i] == message2[i]) {
            new_message[i] = 0;
        }
        else {
            new_message[i] = 1;
        }
    }

    // Free Resources:
    free_vector(message1);
    free_vector(message2);

    return new_message;
}

int* feistel_helper(int* message) {
    // Memory Allocation - Helper:
    int* helper = (int*) malloc(sizeof(int) * 32);
    if (helper == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill Helper:
    for (int i = 0; i < 32; i++) {
        helper[i] = message[i];
    }

    return helper;
}

int** feistel_scheme(int** messages, int** sixteen_keys) {
    int* lr1 = NULL;
    // Memory Allocation - Feistel Messages:
    int** feistel_messages = (int**) malloc(sizeof(int) * 2);
    if (feistel_messages == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initial Arrangement:
    feistel_messages[0] = feistel_helper(messages[1]);
    feistel_messages[1] = operation_xor_feistel(f_function(messages[1], sixteen_keys[0]), messages[0]);

    // Feistel Function:
    for (int i = 1; i < 16; i++, lr1 = NULL) {
        lr1 = feistel_helper(feistel_messages[0]);
        free_vector(feistel_messages[0]);
        feistel_messages[0] = feistel_helper(feistel_messages[1]);
        feistel_messages[1] = operation_xor_feistel(f_function(feistel_messages[1], sixteen_keys[i]), lr1);
    }

    // Free Resources:
    free(messages);
    free(sixteen_keys);

    return feistel_messages;
}

int main() {
    return 0;
}