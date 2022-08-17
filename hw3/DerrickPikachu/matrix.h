#include <iostream>
#include <vector>
#include <mkl.h>

namespace matrix
{

class Matrix {
public:
    Matrix();
    Matrix(int rsize, int csize);
    void test();

    const int & getNrow() const { return nrow; }
    const int & getNcol() const { return ncol; }
    double operator() (size_t row, size_t col) const { return elements[row * ncol + col]; }
    double & operator() (size_t row, size_t col) { return elements[row * ncol + col]; }
    bool operator== (const Matrix & other) const;
    bool operator!= (const Matrix & other) const;
    const double * getBuffer() const { return elements.data(); }
    double * getBuffer() { return elements.data(); }

private:
    int nrow;
    int ncol;
    std::vector<double> elements;
};

Matrix multiply_naive(const Matrix & m1, const Matrix & m2);
Matrix multiply_mkl(const Matrix & m1, const Matrix & m2);

}