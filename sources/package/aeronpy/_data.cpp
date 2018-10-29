#include "_data.hpp"

#include <pybind11/pybind11.h>
#include <fmt/format.h>

using namespace std;
using namespace aeron;
using namespace fmt;
namespace py = pybind11;

int32_t image::session_id(Image& self)
{
    return self.sessionId();
}

bool image::is_end_of_stream(Image& self)
{
    return self.isEndOfStream();
}

bool image::is_closed(Image& self)
{
    return self.isClosed();
}

void image::close(Image& self)
{
    return self.close();
}

string image::__str__(Image& self)
{
    return format("image: session_id:[{}]", self.sessionId());
}

PYBIND11_MODULE(_data, m)
{
    py::class_<Image>(m, "Image")
            .def_property_readonly("session_id", &image::session_id)
            .def_property_readonly("is_end_of_stream", &image::is_end_of_stream)
            .def_property_readonly("is_closed", &image::is_closed)
            .def("close", &image::close)
            .def("__str__", &image::__str__);

    m.attr("NOT_CONNECTED") = NOT_CONNECTED;
    m.attr("BACK_PRESSURED") = BACK_PRESSURED;
    m.attr("ADMIN_ACTION") = ADMIN_ACTION;
    m.attr("PUBLICATION_CLOSED") = PUBLICATION_CLOSED;
    m.attr("MAX_POSITION_EXCEEDED") = MAX_POSITION_EXCEEDED;

}