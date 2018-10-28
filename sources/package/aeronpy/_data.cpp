#include "_data.hpp"

#include <pybind11/pybind11.h>

using namespace aeron;
namespace py = pybind11;


PYBIND11_MODULE(_data, m)
{
    py::class_<Image>(m, "Image");

    m.attr("NOT_CONNECTED") = NOT_CONNECTED;
    m.attr("BACK_PRESSURED") = BACK_PRESSURED;
    m.attr("ADMIN_ACTION") = ADMIN_ACTION;
    m.attr("PUBLICATION_CLOSED") = PUBLICATION_CLOSED;
    m.attr("MAX_POSITION_EXCEEDED") = MAX_POSITION_EXCEEDED;

}