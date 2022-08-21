#include <pybind11/pybind11.h>
#include <vector>
#include <iostream>

#include "matrix.h"

namespace py = pybind11;
using namespace matrix;

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "bind c++ matrix class";

    m.def("multiply_naive", &matrix::multiply_naive);
    m.def("multiply_mkl", &matrix::multiply_mkl);
    m.def("multiply_tile", &matrix::multiply_tile);

    py::class_<Matrix>(m, "Matrix")
        .def(py::init<>())
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::getNrow)
        .def_property_readonly("ncol", &Matrix::getNcol)
        .def("test", &Matrix::test)
        .def("__eq__", [](const Matrix & lmatrix, const Matrix & rmatrix) {
            return lmatrix == rmatrix;
        })
        .def("__ne__", [](const Matrix & lmatrix, const Matrix & rmatrix) {
            return lmatrix != rmatrix;
        })
        .def("__getitem__", [](const Matrix & matrix, py::tuple pos) {
            return matrix(pos[0].cast<int>(), pos[1].cast<int>());
        })
        .def("__setitem__", [](Matrix & matrix, py::tuple pos, float value) {
            matrix(pos[0].cast<int>(), pos[1].cast<int>()) = value;
        });
    
}