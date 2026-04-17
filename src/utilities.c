#include <stdlib.h>
#include "utilities.h"

void free_vector(int* vector) {
    free(vector);
}

void free_matrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
