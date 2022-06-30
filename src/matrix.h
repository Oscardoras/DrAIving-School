#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>


/**
 * @brief Represents a matrix.
 * 
 */
typedef struct {
    float *data;
    unsigned int lines;
    unsigned int columns;
} Matrix;


/**
 * @brief Loads a matrix from a configuration file.
 * 
 * @param file the file containing the matrix.
 * @return the loaded matrix.
 */
Matrix* load_matrix(FILE* file);

/**
 * @brief Saves a matrix into a file.
 * 
 * @param matrix the matrix to save.
 * @param file the file.
 */
void save_matrix(Matrix* matrix, FILE* file);

/**
 * @brief Frees a matrix.
 * 
 * @param matrix the matrix to free.
 */
void free_matrix(Matrix* matrix);


/**
 * @brief Gets a pointer to a matrix element.
 * 
 * @param matrix a pointer to the matrix.
 * @param i the i coordinates.
 * @param j the j coordinates.
 * @return a pointer to the matrix element.
 */
float* get_matrix_element(Matrix* matrix, unsigned int i, unsigned int j);

/**
 * @brief Add two matrix of the same size, and stores the result in the third one
 * 
 * @param q1 one of the matrix to add.
 * @param q2 one of the matrix to add.
 * @param q3 the addition of the other matrix.
 */
void add_matrix(Matrix* q1, Matrix* q2, Matrix* q3);


#endif
