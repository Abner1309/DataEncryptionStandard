#include <stdio.h>
#include <stdlib.h>

int** permuted_choice_1(int* original_key) {
    // Useful Variables:
    int rows = 8, columns = 7, half_key = 28;

    // Memory Allocation: Left Part (C0) + Right Part (D0):
    int** two_keys = (int**) malloc(sizeof(int*) * 2);
    if (two_keys == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(1);
    }
    for (int i = 0; i < 2; i++) {
        two_keys[i] = (int*) malloc(sizeof(int) * half_key);
        if (two_keys[i] == NULL) {
            fprintf(stderr, "Error: Memory could not be allocated.\n");
            exit(1);
        }
    }
    // Memory Allocation: Auxiliary Matrix:
    int** matrix = (int**) malloc(sizeof(int*) * rows);
    if (matrix == NULL) {
        fprintf(stderr, "Error: Memory could not be allocated.\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * columns);
        if (matrix[i] == NULL) {
            fprintf(stderr, "Error: Memory could not be allocated.\n");
            exit(1);
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
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    // Return
    return two_keys;
}

int main() {
    char* number = malloc(sizeof(char) * 57);
    scanf("%56s", number);
    int* digits = (int*) malloc(56 * sizeof(int));
    for (int i = 0; i < 56; i++) {
        digits[i] = number[i] - '0';
    }
    int **matrix = permuted_choice_1(digits);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 28; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n\n");
    }

    for (int i = 0; i < 2; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(digits);
    free(number);
    return 0;
}