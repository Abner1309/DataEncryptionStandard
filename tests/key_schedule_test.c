#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "key_schedule.h"

int main() {
    // Create a String to Storage a Big Number:
    char* number = malloc(sizeof(char) * 57);
    // Accept only 56 bits:
    scanf("%56s", number);
    // Conversion of String in Integer:
    int* digits = (int*) malloc(56 * sizeof(int));
    for (int i = 0; i < 56; i++) {
        digits[i] = number[i] - '0';
    }

    // Call Key Schedule Function:
    int **matrix = key_schedule(digits);

    // Print:
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 48; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    // Free Resources:
    for (int i = 0; i < 16; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(digits);
    free(number);
    return 0;
}
