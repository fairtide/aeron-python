#include "_data.hpp"

#include <pybind11/pybind11.h>

using namespace aeron;
namespace py = pybind11;


PYBIND11_MODULE(_data, m)
{
    py::class_<Image>(m, "Image");
}