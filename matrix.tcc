#include "matrix.h"
#pragma once

// Constructor from initializer list
template <class FLOAT, size_t N>
SquareMatrix<FLOAT, N>::SquareMatrix(std::initializer_list<Vector<FLOAT, N>> values) {
    // First, initialize all columns to zero
    for (size_t i = 0; i < N; ++i) {
        matrix[i] = Vector<FLOAT, N>{};  // Default-constructs to zero vector
    }

    // Then copy the provided values
    size_t i = 0;
    for (const auto& vec : values) {
        if (i >= N) break;
        matrix[i++] = vec;
    }
}

// Non-const column access
template <class FLOAT, size_t N>
Vector<FLOAT, N>& SquareMatrix<FLOAT, N>::operator[](std::size_t i) {
    return matrix[i];
}

// Const column access
template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator[](std::size_t i) const {
    return matrix[i];
}

// Const element access (row, column)
template <class FLOAT, size_t N>
FLOAT SquareMatrix<FLOAT, N>::at(size_t row, size_t column) const {
    return matrix[column][row];
}

// Mutable element access (row, column)
template <class FLOAT, size_t N>
FLOAT& SquareMatrix<FLOAT, N>::at(size_t row, size_t column) {
    return matrix[column][row];
}

// Matrix-vector multiplication
template <class FLOAT, size_t N>
Vector<FLOAT, N> SquareMatrix<FLOAT, N>::operator*(const Vector<FLOAT, N> vector) {
    Vector<FLOAT, N> result{};

    for (size_t row = 0; row < N; ++row) {
        FLOAT sum = static_cast<FLOAT>(0);
        for (size_t col = 0; col < N; ++col) {
            sum += matrix[col][row] * vector[col];
        }
        result[row] = sum;
    }

    return result;
}

// Matrix-matrix multiplication (friend function)
template <class F, size_t K>
SquareMatrix<F, K> operator*(const SquareMatrix<F, K> factor1, const SquareMatrix<F, K> factor2) {
    SquareMatrix<F, K> result{};

    for (size_t col = 0; col < K; ++col) {
        for (size_t row = 0; row < K; ++row) {
            F sum = static_cast<F>(0);
            for (size_t k = 0; k < K; ++k) {
                sum += factor1.matrix[k][row] * factor2.matrix[col][k];
            }
            result.matrix[col][row] = sum;
        }
    }

    return result;
}