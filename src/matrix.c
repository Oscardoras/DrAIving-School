#include <stdlib.h>

#include "matrix.h"


Matrix* load_matrix(FILE* file) {
    Matrix* matrix = malloc(sizeof(Matrix));

    if (matrix != NULL) {
        int n = fscanf(file, "%d %d\n", &matrix->lines, &matrix->columns);
    
        if (n == 2) {
            matrix->data = malloc(sizeof(float) * matrix->lines*matrix->columns);
            
            if (matrix->data != NULL) {
                for (unsigned int i = 0; i < matrix->lines; i++) {
                    char buffer[1024];

                    if (fgets(buffer, 1024, file)) {
                        unsigned int j = 0;
                        char value[256];
                        char* v = value;
                        for (char* c = buffer; *c != '\0'; c++) {
                            if (*c == ' ' || *c == '\n') {
                                *v = '\0';
                                matrix->data[i*matrix->columns + j] = atof(value);

                                v = value;
                                j++;
                            } else {
                                *v = *c;
                                v++;
                            }
                        }
                    } else {
                        free(matrix->data);
                        free(matrix);
                        return NULL;
                    }
                }
            } else {
                free(matrix);
                return NULL;
            }
        } else {
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void save_matrix(Matrix* matrix, FILE* file) {
    fprintf(file, "%d %d\n", matrix->lines, matrix->columns);
    
    for (unsigned int i = 0; i < matrix->lines; i++) {
        for (unsigned int j = 0; j < matrix->columns; j++) {
            if (j == matrix->columns-1)
                fprintf(file, "%f\n", *get_matrix_element(matrix, i, j));
            else
                fprintf(file, "%f ", *get_matrix_element(matrix, i, j));
        }
    }
            
}

void free_matrix(Matrix* matrix) {
    if (matrix != NULL) {
        if (matrix->data != NULL) {
            free(matrix->data);
            matrix->data = NULL;
        }
        
        free(matrix);
    }
}

float* get_matrix_element(Matrix* matrix, unsigned int i, unsigned int j) {
    return &matrix->data[i*matrix->columns + j];
}

void add_matrix(Matrix* q1, Matrix* q2, Matrix* r) {
    for (unsigned int i = 0; i < q1->lines; i++)
        for (unsigned int j = 0; j < q1->columns; j++)
            *get_matrix_element(r, i, j) = *get_matrix_element(q1, i, j) + *get_matrix_element(q2, i, j);
}
