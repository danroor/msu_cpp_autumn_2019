#include "matrix.h"

Matrix::Row::Row() {}; //used in matrix copy constructor to create an empty row
Matrix::Row::Row(size_t cols) : ncols(cols)
{
    elem.resize(ncols, 0);
}
Matrix::Row::Row(const Row &r) : ncols(r.ncols)
{
    elem.resize(ncols, 0);
    for (size_t i = 0; i < ncols; ++i)
        elem[i] = r.elem[i];
}

Matrix::Matrix(size_t row, size_t col) : nrows(row), ncols(col)
{
    rows.resize(nrows, Row(ncols));
}

Matrix::Matrix(const Matrix &m) : nrows(m.nrows), ncols(m.ncols)
{
    rows.resize(nrows);
    for (size_t i = 0; i < nrows; ++i) {
        (*this)[i] = Row(m[i]);
    }
}

size_t Matrix::getRows() const { return nrows; }
size_t Matrix::getCols() const { return ncols; }

Matrix Matrix::operator*=(int x) {
    for (size_t i = 0; i < nrows; ++i)
       for (size_t j = 0; j < ncols; ++j)
          (*this)[i][j] *= x;
    return *this;
}

const Matrix::Row & Matrix::operator[](size_t row) const {
    if (row >= nrows)
        throw std::out_of_range("Row index out of range");
    return rows[row];
}

const int & Matrix::Row::operator[](size_t col) const {
    if (col >= ncols)
        throw std::out_of_range("Column index out of range"); 
    return elem[col];
}

Matrix::Row & Matrix::operator[](size_t row) {
    if (row >= nrows)
        throw std::out_of_range("Row index out of range");
    return rows[row];
}

int & Matrix::Row::operator[](size_t col) {
    if (col >= ncols)
        throw std::out_of_range("Column index out of range"); 
    return elem[col];
}

bool Matrix::operator!=(const Matrix &m) const {
    if (nrows != m.nrows || ncols != m.ncols) return true;
    for (size_t i = 0; i < nrows; ++i)
        for (size_t j = 0; j < ncols; ++j)
            if ( (*this)[i][j] != m[i][j]) return true;
    return false;
}

bool Matrix::operator==(const Matrix &m) const {
    return !(*this != m);
}
