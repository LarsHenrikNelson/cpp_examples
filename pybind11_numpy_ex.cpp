#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void mutable_access(py::array_t<double> array)
{
    // direct access to numpy data, can modify in place.
    auto r = array.mutable_unchecked<1>();
    std::cout << r.shape(0) << "\n";
    for (long unsigned int i = 0; i < r.shape(0); i++)
    {
        std::cout << r(i) << "\n";
        r(i) += 1;
    }
}

void access(py::array_t<double> array)
{
    // access to data, but cannot modify inplace.
    for (long unsigned int i = 0; i < array.shape(0); i++)
    {
        std::cout << *array.data(i) << "\n";
    }
}

PYBIND11_MODULE(pybind11_numpy_ex, m)
{
    m.doc() = "Test for numpy array to C++"; // optional module docstring

    m.def("mutable_access", &mutable_access);
    m.def("access", &access);
}