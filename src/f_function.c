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

int** divide_for_s_boxes(int* right_half) {
    // Memory Allocation - Eight Messages (6 Bits):
    int** eight_messages = (int**) malloc(sizeof(int*) * 8);
    if (eight_messages == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 8; i++) {
        eight_messages[i] = (int*) malloc(sizeof(int) * 6);
        if (eight_messages[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Fill In The Eight Messages:
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            eight_messages[i][j] = right_half[i * 6 + j];
        }
    }

    // Free Resources:
    free_vector(right_half);

    return eight_messages;
}

int binary_to_integer(int binary, int size) {
    int sum = 0;
    for (int i = 0, current = 0, exp = 1; i < size; i++, exp = 1) {
        current = binary % 10;
        binary = binary / 10;
        for (int j = 0; j < i; j++) {
            exp = exp * 2;
        }
        sum = sum + current * exp;
    }
    return sum;
}

void integer_to_binary(int* chunk, int integer) {
    int i = 3, quotient = integer;
    while (quotient != 0) {
        chunk[i] = quotient % 2;
        quotient = quotient / 2;
        i--;
    }
    while (i > -1) {
        chunk[i] = 0;
        i--;
    }
}

int* s_box(int* chunk, int index) {
    // Definition of Row Index:
    int row_index = 0;
    row_index = binary_to_integer(chunk[0] * 10 + chunk[5], 2);

    // Definition of Column Index:
    int column_index = 0;
    column_index = binary_to_integer(chunk[1] * 1000 + chunk[2] * 100 + chunk[3] * 10 + chunk[4], 4);

    // Memory Allocation - New Chunk:
    int* new_chunk = (int*) malloc(sizeof(int) * 4);
    if (new_chunk == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Find S-Box Element:
    int element;
    if (index == 1) { element = S_BOX_1[row_index][column_index]; }
    else if (index == 2) { element = S_BOX_2[row_index][column_index]; }
    else if (index == 3) { element = S_BOX_3[row_index][column_index]; }
    else if (index == 4) { element = S_BOX_4[row_index][column_index]; }
    else if (index == 5) { element = S_BOX_5[row_index][column_index]; }
    else if (index == 6) { element = S_BOX_6[row_index][column_index]; }
    else if (index == 7) { element = S_BOX_7[row_index][column_index]; }
    else if (index == 8) { element = S_BOX_8[row_index][column_index]; }
    else {
        fprintf(stderr, "Invalid index\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Chunk:
    for (int i = 0; i < 4; i++) {
        integer_to_binary(new_chunk, element);
    }

    return new_chunk;
}

int** operation_s_boxes(int** eight_messages) {
    // Memory Allocation - Result:
    int** result = (int**) malloc(sizeof(int) * 8);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // S-Box Loop:
    for (int i = 0; i < 8; i++) {
        result[i] = s_box(eight_messages[i], i + 1);
    }

    // Free Resources:
    free_matrix(eight_messages, 8);

    return result;
}

int* join_messages(int** messages) {
    // Memory Allocation - New Right Half:
    int* new_right_half = (int*) malloc(sizeof(int) * 32);
    if (new_right_half == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Right Half:
    for (int i = 0, j = 0, k = 0; i < 32; i++, k++) {
        if (k > 3) {
            k = 0;
            j++;
        }
        new_right_half[i] = messages[j][k];
    }

    // Free Resources:
    free_matrix(messages, 8);

    return new_right_half;
}

int* operation_p_box(int* right_half) {
    // P-Box:
    int p_box[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10,
                 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25};

    // Memory Allocation - New Right Half:
    int* new_right_half = (int*) malloc(sizeof(int) * 32);
    if (new_right_half == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill New Right Half:
    for (int i = 0; i < 32; i++) {
        new_right_half[i] = right_half[p_box[i] - 1];
    }

    // Free Resources:
    free_vector(right_half);

    return new_right_half;
}

int* f_function(int* right_half, int* key) {
    int* a1 = expansion(right_half);
    int* a2 = operation_xor(a1, key);
    int** a3 = divide_for_s_boxes(a2);
    int** a4 = operation_s_boxes(a3);
    int* a5 = join_messages(a4);
    int* a6 = operation_p_box(a5);
    return a6;
}
