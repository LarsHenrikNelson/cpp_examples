#include <numeric>
#include <vector>

#include <pcg_random.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "bootstrap_header.hpp"

namespace py = pybind11;

template <class T>
std::vector<T> numpy_to_vec(py::array_t<T> py_array)
{
    return std::vector<T>(py_array.data(), py_array.data() + py_array.size());
}

double bootstrap_two_sample(py::array_t<double> &array1, py::array_t<double> &array2, int repeats = 10000, int seed = 42)
{
    auto vec1 = numpy_to_vec<double>(array1);
    auto vec2 = numpy_to_vec<double>(array2);
    auto p_value = bootstrap_test(vec1, vec2, repeats, seed);
    return p_value;
}

PYBIND11_MODULE(bootstrap_test, m)
{
    m.doc() = "bootstrap test for two samples";

    m.def("bootstrap_two_sample", &bootstrap_two_sample, "Bootstraps two samples to find p_value difference.");
}