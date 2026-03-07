#include "hw1.h"
#include <random>
#include <iomanip>
#include <iostream>

using Matrix = std::vector<std::vector<double>>;

namespace algebra {

    Matrix zeros(size_t n, size_t m) {
        Matrix matrix(n, vector<double>(m, 0));
        return matrix;
    }

    Matrix ones(size_t n, size_t m) {
        Matrix matrix(n, vector<double>(m, 1));
        return matrix;
    }

    Matrix random(size_t n, size_t m, double min, double max) {
        if (min > max) throw std::logic_error("min cannot be greater than max");
        Matrix matrix(n, vector<double>(m));

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_real_distribution<double> dist(min, max);
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < m; j++)
                matrix[i][j] = dist(gen);
        return matrix;
    }

    void show(const Matrix& matrix) {
        for (const auto& row : matrix) {
            for (const auto& elem : row)
                std::cout << std::fixed << std::setprecision(3)
                          << std::setw(10) << elem << ' ';
            std::cout << '\n';
        }
    }

    Matrix multiply(const Matrix& matrix, double c) {
        Matrix result(matrix.size(), vector<double>(matrix[0].size()));
        for (size_t i = 0; i < matrix.size(); i++)
            for (size_t j = 0; j < matrix[i].size(); j++)
                result[i][j] = matrix[i][j] * c;
        return result;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty() && matrix2.empty()) return {};
        if (matrix1[0].size() != matrix2.size())
            throw std::logic_error("matrices with wrong dimensions cannot be multiplied");
        Matrix matrix(matrix1.size(), vector<double>(matrix2[0].size(), 0));
        for (size_t i = 0; i < matrix1.size(); i++)
            for (size_t j = 0; j < matrix2[0].size(); j++)
                for (size_t k = 0; k < matrix1[0].size(); k++)
                    matrix[i][j] += matrix1[i][k] * matrix2[k][j];
        return matrix;
    }

    Matrix sum(const Matrix& matrix, double c) {
        if (matrix.empty()) return {};
        Matrix result(matrix.size(), vector<double>(matrix[0].size()));
        for (size_t i = 0; i < matrix.size(); i++)
            for (size_t j = 0; j < matrix[0].size(); j++)
                result[i][j] = matrix[i][j] + c;
        return result;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty() && matrix2.empty()) return {};
        if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size())
            throw std::logic_error("matrices with wrong dimensions cannot be summed");
        Matrix matrix(matrix1.size(), vector<double>(matrix2[0].size(), 0));
        for (size_t i = 0; i < matrix1.size(); i++)
            for (size_t j = 0; j < matrix1[i].size(); j++)
                matrix[i][j] = matrix1[i][j] + matrix2[i][j];
        return matrix;
    }

    Matrix transpose(const Matrix& matrix) {
        if (matrix.empty()) return {};
        Matrix result(matrix[0].size(), vector<double>(matrix.size()));
        for (size_t i = 0; i < matrix.size(); i++)
            for (size_t j = 0; j < matrix[i].size(); j++)
                result[j][i] = matrix[i][j];
        return result;
    }

    Matrix minor(const Matrix& matrix, size_t n, size_t m) {
        Matrix result(matrix.size() - 1, vector<double>(matrix[0].size() - 1));
        for (size_t i = 0; i < matrix.size(); i++) {
            if (i == n) continue;
            for (size_t j = 0; j < matrix[i].size(); j++) {
                if (j == m) continue;
                result[i < n ? i : i - 1][j < m ? j : j - 1] = matrix[i][j];
            }
        }
        return result;
    }

    double determinant(const Matrix& matrix) {
        double result = 0;
        if (matrix.empty()) return 1;
        if (matrix.size() != matrix[0].size())
            throw std::logic_error("non-square matrices have no determinant");
        if (matrix.size() == 1 && matrix[0].size() == 1) return matrix[0][0];
        for (size_t i = 0; i < matrix.size(); i++) {
           int coff = (i + 0) % 2 == 0 ? 1 : -1;
           result += coff * matrix[i][0] * determinant(minor(matrix, i, 0));
        }
        return result;
    }

    Matrix inverse(const Matrix& matrix) {
        if (matrix.empty()) return {};
        if (matrix.size() != matrix[0].size())
            throw std::logic_error("non-square matrices have no inverse");
        double det = determinant(matrix);
        const double eps = 1e-12;
        if (std::abs(det) < eps) {
            throw std::logic_error("Singular matrix has no inverse");
        }

        Matrix adj(matrix.size(), vector<double>(matrix.size()));
        for (size_t i = 0; i < matrix.size(); i++) {
            for (size_t j = 0; j < matrix[0].size(); j++) {
                Matrix sub = minor(matrix, i, j);
                double cofactor = ((i + j) % 2 == 0 ? 1.0 : -1.0) * determinant(sub);
                adj[j][i] = cofactor;
            }
        }
        return multiply(adj,  1.0 / det);
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
        if (axis == 0) {
            if (matrix1[0].size() != matrix2[0].size())
                throw std::logic_error("Matrices with wrong dimensions cannot be concatenated");
            Matrix result(matrix1.size() + matrix2.size(), vector<double>(matrix1[0].size()));
            for (size_t i = 0; i < matrix1.size(); i++)
                for (size_t j = 0; j < matrix1[0].size(); j++)
                    result[i][j] = matrix1[i][j];
            for (size_t i = 0; i < matrix2.size(); i++)
                for (size_t j = 0; j < matrix2[0].size(); j++)
                    result[matrix1.size() + i][j] = matrix2[i][j];
            return result;
        } else {
            if (matrix1.size() != matrix2.size())
                throw std::logic_error("Matrices with wrong dimensions cannot be concatenated");
            Matrix result(matrix1.size(), vector<double>(matrix1[0].size() + matrix2[0].size()));
            for (size_t i = 0; i < matrix1.size(); i++)
                for (size_t j = 0; j < matrix1[0].size(); j++)
                    result[i][j] = matrix1[i][j];
            for (size_t i = 0; i < matrix2.size(); i++)
                for (size_t j = 0; j < matrix2[0].size(); j++)
                    result[i][j + matrix1[0].size()] = matrix2[i][j];
             return result;
        }
    }

    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
        if (r1 >= matrix.size() || r2 >= matrix.size())
            throw std::logic_error("r1 or r2 inputs are out of range");
        Matrix result(matrix);
        std::swap(result[r1], result[r2]);
        return result;
    }

    Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
        Matrix result(matrix);
        for (size_t j = 0; j < matrix[r].size(); j++)
            result[r][j] *= c;
        return result;
    }

    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
        Matrix result(matrix);
        for (size_t j = 0; j < matrix[0].size(); j++)
            result[r2][j] = result[r1][j] * c + result[r2][j];
        return result;
    }

    Matrix upper_triangular(const Matrix& matrix) {
        if (matrix.empty()) return {};
        if (matrix.size() != matrix[0].size())
            throw std::logic_error("non-square matrices cannot be transformed into upper triangular form");
        Matrix result(matrix);
        const double eps = 1e-12;
        for (size_t i = 0; i < matrix.size(); i++) {
            for (size_t j = i; j < matrix[0].size(); j++) {
                if (std::abs(result[i][i]) < eps) {
                    size_t swap_row = i + 1;
                    while (swap_row < matrix.size() && std::abs(result[swap_row][i]) < eps) {
                        swap_row++;
                    }
                    if (swap_row == matrix.size()) continue; // Cannot eliminate this column
                    result = ero_swap(result, i, swap_row);
                }
                for (size_t k = i + 1; k < matrix.size(); k++) {
                    double factor = result[k][i] / result[i][i];
                    result = ero_sum(result, i, -factor, k);
                }
            }
        }
        return result;
    }
}

