#include <stdlib.h>
#include "utilities.h"

#include <stdio.h>

void free_vector(int* vector) {
    free(vector);
    vector = NULL;
}

void free_matrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
        matrix[i] = NULL;
    }
    free(matrix);
    matrix = NULL;
}

void print_vector(int* vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", vector[i]);
    }
    printf("\n");
}

void print_matrix(int** matrix, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printf("%d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
