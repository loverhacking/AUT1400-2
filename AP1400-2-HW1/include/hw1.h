#ifndef AP_HW1_H
#define AP_HW1_H

#include <vector>
using Matrix = std::vector<std::vector<double>>;

namespace algebra {
    using std::size_t;
    using std::vector;

    // create a n x m matrix with all elements equal to zero.
    Matrix zeros(size_t n, size_t m);

    // create a n x m matrix with all elements equal to one.
    Matrix ones(size_t n, size_t m);

    // create a n x m matrix with all elements a random number between min and max.
    Matrix random(size_t n, size_t m, double min, double max);

    // display the matrix in a beautiful way
    void show(const Matrix& matrix);

    // multiplies the matrix into the constant scalar c
    Matrix multiply(const Matrix& matrix, double c);

    // multiplies the matrix1 into matrix2
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);

    // adds the constant number c to every element of matrix
    Matrix sum(const Matrix& matrix, double c);

    // adds 2 matrices to each other
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2);

    // generate the transpose matrix of the input matrix
    Matrix transpose(const Matrix& matrix);

    // create the minor of the input matrix with respect to nth row and mth column
    Matrix minor(const Matrix& matrix, size_t n, size_t m);

    // calculates the determinant of the input matrix
    double determinant(const Matrix& matrix);

    // generates the matrix's inverse
    Matrix inverse(const Matrix& matrix);

    // concatenate matrix1 and matrix2 along the specified axis.
    // (axis=0: on top of each other | axis=1: alongside each other).
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0);

    // swapping two rows
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2);

    // multiplying a row into a constant number
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c);

    // multiplying a row into a constant number and add it to another row
    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2);

    // calculate the upper triangular form of the matrix using the ERO operations
    Matrix upper_triangular(const Matrix& matrix);
}


#endif //AP_HW1_H
