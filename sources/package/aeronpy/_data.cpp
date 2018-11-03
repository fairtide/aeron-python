/*
 * Copyright 2018 Fairtide Pte. Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

int32_t header::stream_id(Header& self)
{
    return self.streamId();
}

int32_t header::session_id(Header& self)
{
    return self.sessionId();
}

string header::__str__(Header& self)
{
    return format("header: stream_id:[{}] session_id:[{}]", self.streamId(), self.sessionId());
}

PYBIND11_MODULE(_data, m)
{
    py::class_<Image>(m, "Image")
            .def_property_readonly("session_id", &image::session_id)
            .def_property_readonly("is_end_of_stream", &image::is_end_of_stream)
            .def_property_readonly("is_closed", &image::is_closed)
            .def("close", &image::close)
            .def("__str__", &image::__str__);

    py::class_<Header>(m, "Header")
            .def_property_readonly("stream_id", &header::session_id)
            .def_property_readonly("session_id", &header::session_id)
            .def("__str__", &header::__str__);

    m.attr("NOT_CONNECTED") = NOT_CONNECTED;
    m.attr("BACK_PRESSURED") = BACK_PRESSURED;
    m.attr("ADMIN_ACTION") = ADMIN_ACTION;
    m.attr("PUBLICATION_CLOSED") = PUBLICATION_CLOSED;
    m.attr("MAX_POSITION_EXCEEDED") = MAX_POSITION_EXCEEDED;

}