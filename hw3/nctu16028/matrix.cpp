#include "matrix.hpp"
#include <mkl.h>
#include <pybind11/pybind11.h>

// default constructor
Matrix::Matrix()
{
    m_nrow = 0;
    m_ncol = 0;
    m_buffer = nullptr;
}

// copy constructor
Matrix::Matrix(Matrix const &cp)
{
    m_nrow = cp.m_nrow;
    m_ncol = cp.m_ncol;

    //if(m_buffer)
    //    delete[] m_buffer;
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement]();
    for(size_t i=0; i<nelement; i++)
        m_buffer[i] = cp.m_buffer[i];
}

// move constructor
Matrix::Matrix(Matrix &&mv)
{
    m_nrow = mv.m_nrow;
    m_ncol = mv.m_ncol;
    m_buffer = mv.m_buffer;
    mv.m_buffer = nullptr;
}

// copy assignment operator
Matrix & Matrix::operator=(Matrix const &cp_rval)
{
    if(this == &cp_rval)
        return *this;

    m_nrow = cp_rval.m_nrow;
    m_ncol = cp_rval.m_ncol;

    //if(m_buffer)
    //    delete[] m_buffer;
    size_t nelement = m_nrow * m_ncol;
    m_buffer = new double[nelement]();
    for(size_t i=0; i<nelement; i++)
        m_buffer[i] = cp_rval.m_buffer[i];

    return *this;
}

// move assignment operator
Matrix & Matrix::operator=(Matrix &&mv_rval)
{
    if(this == &mv_rval)
        return *this;

    m_nrow = mv_rval.m_nrow;
    m_ncol = mv_rval.m_ncol;
    m_buffer = mv_rval.m_buffer;
    mv_rval.m_buffer = nullptr;

    return *this;
}

// custom constructor
Matrix::Matrix(size_t nrow, size_t ncol)
{
    m_nrow = nrow;
    m_ncol = ncol;
    size_t nelement = nrow * ncol;
    m_buffer = new double[nelement]();
}

// destructor
Matrix::~Matrix()
{
    if(m_buffer)
        delete[] m_buffer;
}

// access the number of rows
size_t Matrix::nrow() const
{
    return m_nrow;
}

// access the number of columns
size_t Matrix::ncol() const
{
    return m_ncol;
}

// access the raw contents of the buffer
double* Matrix::buff() const
{
    return m_buffer;
}

// element getter
double Matrix::operator() (size_t row, size_t col) const
{
    return m_buffer[row*m_ncol + col];
}

// element setter
double & Matrix::operator() (size_t row, size_t col)
{
    return m_buffer[row*m_ncol + col];
}

// equality operator overloading
bool Matrix::operator==(Matrix const &other)
{
    if((m_nrow != other.nrow()) || (m_ncol != other.ncol()))
        return false;

    for(size_t i=0; i<m_nrow; i++)
    {
        for(size_t j=0; j<m_ncol; j++)
        {
            if((*this)(i, j) != other(i, j))
                return false;
        }
    }

    return true;
}

// Perform naive matrix-matrix multiplication
Matrix multiply_naive(Matrix const &matA, Matrix const &matB)
{
    Matrix matC(matA.nrow(), matB.ncol());
    for(size_t i=0; i<matA.nrow(); i++)
    {
        for(size_t j=0; j<matB.ncol(); j++)
        {
            double val = 0;
            for(size_t k=0; k<matA.ncol(); k++)
                val += (matA(i, k) * matB(k, j));
            matC(i, j) = val;
        }
    }
    return matC;
}

// Perform matrix-matrix multiplication with tiling
Matrix multiply_tile(Matrix const &matA, Matrix const &matB, size_t const tile_size)
{
    Matrix matC(matA.nrow(), matB.ncol());
    for(size_t i=0; i<matA.nrow(); i+=tile_size)
    {
        for(size_t j=0; j<matB.ncol(); j+=tile_size)
        {
            for(size_t k=0; k<matA.ncol(); k+=tile_size)
            {
                // tile-tile multiplication
                size_t ti_bound = std::min(i+tile_size, matA.nrow());
                size_t tj_bound = std::min(j+tile_size, matB.ncol());
                size_t tk_bound = std::min(k+tile_size, matA.ncol());
                for(size_t ti=i; ti<ti_bound; ti++)
                {
                    for(size_t tj=j; tj<tj_bound; tj++)
                    {
                        double val = 0;
                        for(size_t tk=k; tk<tk_bound; tk++)
                            val += (matA(ti, tk) * matB(tk, tj));
                        matC(ti, tj) += val;
                    }
                }
            }
        }
    }
    return matC;
}

// Call DGEMM for the multiplication
Matrix multiply_mkl(Matrix const &matA, Matrix const &matB)
{
    //mkl_set_num_threads(1);

    Matrix matC(matA.nrow(), matB.ncol());

    cblas_dgemm(
        CblasRowMajor,  /* const CBLAS_LAYOUT Layout */
        CblasNoTrans,   /* const CBLAS_TRANSPOSE transa */
        CblasNoTrans,   /* const CBLAS_TRANSPOSE transb */
        matA.nrow(),    /* const MKL_INT m */
        matB.ncol(),    /* const MKL_INT n */
        matA.ncol(),    /* const MKL_INT k */
        1.0,            /* const double alpha */
        matA.buff(),    /* const double *a */
        matA.ncol(),    /* const MKL_INT lda */
        matB.buff(),    /* const double *b */
        matB.ncol(),    /* const MKL_INT ldb */
        0.0,            /* const double beta */
        matC.buff(),    /* double * c */
        matC.ncol()     /* const MKL_INT ldc */
    );

    return matC;
}

PYBIND11_MODULE(_matrix, m)
{
    m.def("multiply_naive", &multiply_naive);
    m.def("multiply_tile", &multiply_tile);
    m.def("multiply_mkl", &multiply_mkl);

    pybind11::class_<Matrix>(m, "Matrix")
        .def(pybind11::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__",
            [](Matrix &mat, std::pair<size_t, size_t> key) {
                return mat(key.first, key.second);
            })
        .def("__setitem__",
            [](Matrix &mat, std::pair<size_t, size_t> key, double val) {
                mat(key.first, key.second) = val;
            })
        .def("__eq__", &Matrix::operator==);
}

