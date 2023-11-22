#include <iostream>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

py::array_t<double> rolling_rms(py::array_t<double> array, py::ssize_t window)
{

    // auto r = array.mutable_unchecked<1>();
    py::ssize_t end = array.size() - window;

    auto result = py::array_t<double>(end);
    py::buffer_info output_buf = result.request();

    double *output_ptr = static_cast<double *>(output_buf.ptr);

    for (py::ssize_t i = 0; i < end; ++i)
    {
        double temp{0.0};

        for (py::ssize_t j = 0; j < window; ++j)
        {
            temp += *array.data(i + j) * *array.data(i + j);
        }
        output_ptr[i] = std::sqrt(temp / static_cast<double>(window));
    }

    return result;
}

PYBIND11_MODULE(rolling_rms, m)
{
    m.doc() = "Rolling RMS written in C++"; // optional module docstring

    m.def("rolling_rms", &rolling_rms);
}