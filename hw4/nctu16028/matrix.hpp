#pragma once
#include <cstddef>
#include "alloc.hpp"

class Matrix
{
public:
    Matrix();                               // default construtor
    Matrix(Matrix const & );                // copy constructor
    Matrix(Matrix       &&);                // move constructor
    Matrix & operator=(Matrix const & );    // copy assignment operator
    Matrix & operator=(Matrix       &&);    // move assignment operator
    Matrix(size_t nrow, size_t ncol);       // custom constructor
    ~Matrix();                              // destructor
    size_t nrow() const;                    // access the number of rows
    size_t ncol() const;                    // access the number of columns
    double   operator() (size_t row, size_t col) const; // element getter
    double & operator() (size_t row, size_t col);       // element setter
    bool operator==(Matrix const &);        // equality operator overloading
    std::vector<double, CustomAllocator<double>> m_buffer;

private:
    size_t m_nrow;
    size_t m_ncol;
}; /* end of class Matrix */

// Free functions for multiplication
Matrix multiply_naive(Matrix const &matA, Matrix const &matB);
Matrix multiply_tile(Matrix const &matA, Matrix const &matB, size_t const tile_size);
Matrix multiply_mkl(Matrix const &matA, Matrix const &matB);

// ByteCounter accessor
size_t bytes();
size_t allocated();
size_t deallocated();

