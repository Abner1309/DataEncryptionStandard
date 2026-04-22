#include <stdlib.h>
#include "utilities.h"

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
