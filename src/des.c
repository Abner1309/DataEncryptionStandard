#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int** feistel_scheme_encryption(int** messages, int** sixteen_keys) {
    int* lr1 = NULL;
    // Memory Allocation - Feistel Messages:
    int** feistel_messages = (int**) malloc(sizeof(int*) * 2);
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

int** feistel_scheme_decryption(int** messages, int** sixteen_keys) {
    int* lr1 = NULL;
    // Memory Allocation - Feistel Messages:
    int** feistel_messages = (int**) malloc(sizeof(int*) * 2);
    if (feistel_messages == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initial Arrangement:
    feistel_messages[0] = feistel_helper(messages[1]);
    feistel_messages[1] = operation_xor_feistel(f_function(messages[1], sixteen_keys[15]), messages[0]);

    // Feistel Function:
    for (int i = 14; i > -1; i--, lr1 = NULL) {
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

int* join_message_des(int** messages) {
    // Memory Allocation - Joined Message:
    int* joined_message = (int*) malloc(sizeof(int) * 64);
    if (joined_message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill Joined Message:
    for (int i = 0, j = 0, k = 0; i < 64; i++, k++) {
        if (k > 31) {
            k = 0;
            j++;
        }
        joined_message[i] = messages[j][k];
    }

    // Free Resources:
    free_matrix(messages, 2);

    return joined_message;
}

int* join_message_des_inverted(int** messages) {
    // Memory Allocation - Joined Message:
    int* joined_message = (int*) malloc(sizeof(int) * 64);
    if (joined_message == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill Joined Message:
    for (int i = 0, j = 1, k = 0; i < 64; i++, k++) {
        if (k > 31) {
            k = 0;
            j--;
        }
        joined_message[i] = messages[j][k];
    }

    // Free Resources:
    free_matrix(messages, 2);

    return joined_message;
}

int* final_permutation(int* message) {
    // Auxiliary Matrix:
    int matrix[8][8] = {
        {40, 8, 48, 16, 56, 24, 64, 32},
        {39, 7, 47, 15, 55, 23, 63, 31},
        {38, 6, 46, 14, 54, 22, 62, 30},
        {37, 5, 45, 13, 53, 21, 61, 29},
        {36, 4, 44, 12, 52, 20, 60, 28},
        {35, 3, 43, 11, 51, 19, 59, 27},
        {34, 2, 42, 10, 50, 18, 58, 26},
        {33, 1, 41, 9, 49, 17, 57, 25}
    };

    // Memory Allocation - Encrypted Text:
    int* encrypted_text = (int*) malloc(sizeof(int) * 64);
    if (encrypted_text == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Fill Encrypted Text:
    for (int i = 0, j = 0, k = 0; i < 64; i++, k++) {
        if (k > 7) {
            k = 0;
            j++;
        }
        encrypted_text[i] = message[matrix[j][k] - 1];
    }

    // Free Resources:
    free(message);

    return encrypted_text;
}

int main() {
    // Ask For Files:
    char input_file_path[512];
    char output_file_path[512];
    printf("Enter the path to the input file: ");
    fgets(input_file_path, sizeof(input_file_path), stdin);
    input_file_path[strcspn(input_file_path, "\n")] = '\0';
    printf("Enter the path to the output file: ");
    fgets(output_file_path, sizeof(output_file_path), stdin);
    output_file_path[strcspn(output_file_path, "\n")] = '\0';

    // Open Files:
    FILE* arq_input = fopen(input_file_path, "r");
    FILE* arq_output = fopen(output_file_path, "w");
    if (arq_input == NULL || arq_output == NULL) {
        printf("Error When Opening Files");
        return 1;
    }

    // Read Mode, Key and Message:
    char mode[4] = "";
    int key[56] = {0};
    int message[64] = {0};
    int c = 0, i = 0;
    while (i < 3 && (c = fgetc(arq_input)) != EOF) {
        if (c == 'e' || c == 'n' || c == 'c' || c == 'd') {
            mode[i] = (char) c;
            i++;
        }
        else if (c == '\n' || c == '\r' || c == ' ') {
            continue;
        }
        else {
            fprintf(stderr, "Error: Invalid character found: %c\n", c);
            exit(EXIT_FAILURE);
        }
    }
    i = 0;
    while (i < 56 && (c = fgetc(arq_input)) != EOF) {
        if (c == '0' || c == '1') {
            key[i] = c - '0';
            i++;
        }
        else if (c == '\n' || c == '\r' || c == ' ') {
            continue;
        }
        else {
            fprintf(stderr, "Error: Invalid character found: %c\n", c);
            exit(EXIT_FAILURE);
        }
    }
    i = 0;
    while (i < 64 && (c = fgetc(arq_input)) != EOF) {
        if (c == '0' || c == '1') {
            message[i] = c - '0';
            i++;
        }
        else if (c == '\n' || c == '\r' || c == ' ') {
            continue;
        }
        else {
            fprintf(stderr, "Error: Invalid character found: %c\n", c);
            exit(EXIT_FAILURE);
        }
    }

    // Data Encryption Standard - Encryption:
    if (strcmp(mode, "enc") == 0) {
        int** a1 = key_schedule(key);
        int* a2 = initial_permutation(message);
        int** a3 = divide_message(a2);
        int** a4 = feistel_scheme_encryption(a3, a1);
        int* a5 = join_message_des_inverted(a4);
        int* a6 = final_permutation(a5);

        // Write Answer:
        fprintf(arq_output, "Ciphertext: ");
        for (int x = 0; x < 64; x++) {
            fprintf(arq_output, "%d", a6[x]);
        }

        // Free Resources:
        free_vector(a6);
    }
    // Data Encryption Standard - Decryption:
    else if (strcmp(mode, "dec") == 0) {
        int** a1 = key_schedule(key);
        int* a2 = initial_permutation(message);
        int** a3 = divide_message(a2);
        int** a4 = feistel_scheme_decryption(a3, a1);
        int* a5 = join_message_des_inverted(a4);
        int* a6 = final_permutation(a5);

        // Write Answer:
        fprintf(arq_output, "Original Text: ");
        for (int x = 0; x < 64; x++) {
            fprintf(arq_output, "%d", a6[x]);
        }

        // Free Resources:
        free_vector(a6);
    }
    // Error!
    else {
        printf("The operation mode should be 'enc' or 'dec'\n");
        exit(EXIT_FAILURE);
    }

    // Free Resources:
    fclose(arq_input);
    fclose(arq_output);
    return 0;
}