#ifndef STDLIBS
#define STDLIBS

#include <cstdlib>
#include <iostream>

#endif

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {
    size_t nrows, ncols;

    class Row {
        size_t ncols;
        std::vector<int> elem;
    public:
        Row();
        Row(size_t cols);
        Row(const Row &r);

        int & operator[](size_t col);
        const int & operator[](size_t col) const; //for const matrices
    };

    std::vector<Row> rows;

public:
    Matrix(size_t row, size_t col);
    Matrix(const Matrix &m);
    size_t getRows() const;
    size_t getCols() const;

    Matrix operator*=(int x);
    Row & operator[](size_t row);
    const Row & operator[](size_t row) const; //for const matrices

    bool operator!=(const Matrix &m) const;
    bool operator==(const Matrix &m) const;
};

#endif
