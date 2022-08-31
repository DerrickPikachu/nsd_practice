#include <iostream>
#include <vector>
#include <atomic>

namespace matrix
{

template <class T>
class MatrixAllocatorCounter {
public:
    using value_type = T;

    static std::atomic_size_t allocate_bytes;
    static std::atomic_size_t deallocate_bytes;

    T* allocate(size_t num_element);
    void deallocate(T* ptr, size_t num_element) noexcept;
};

class Matrix {
public:
    Matrix();
    ~Matrix();
    Matrix(int rsize, int csize);

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
    // std::vector<double, MatrixAllocatorCounter<double>> elements;
    std::vector<double> elements;
};

Matrix multiply_naive(const Matrix & m1, const Matrix & m2);
Matrix multiply_mkl(const Matrix & m1, const Matrix & m2);
Matrix multiply_tile(const Matrix & m1, const Matrix & m2, int tsize);
void multiply_block(Matrix & res, const Matrix & m1, const Matrix & m2, int row, int col, int multiply_iter, int tsize);
int bytes();
int allocated();
int deallocated();

}