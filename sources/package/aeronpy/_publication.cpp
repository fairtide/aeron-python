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

#include "_publication.hpp"

#include <Aeron.h>
#include <fmt/format.h>
#include <pybind11/pybind11.h>

using namespace std;
using namespace aeron;
using namespace fmt;
namespace py = pybind11;


publication::publication(shared_ptr<Publication> aeron_publication)
    :
        aeron_publication_(aeron_publication)
{

}

const string& publication::channel() const
{
    return aeron_publication_->channel();
}

int64_t publication::stream_id() const
{
    return aeron_publication_->streamId();
}

int32_t publication::session_id() const
{
    return aeron_publication_->sessionId();
}

int32_t publication::initial_term_id() const
{
    return aeron_publication_->initialTermId();
}

bool publication::is_connected() const
{
    return aeron_publication_->isConnected();
}

bool publication::is_closed() const
{
    return aeron_publication_->isClosed();
}

bool publication::is_original() const
{
    return aeron_publication_->isOriginal();
}

int64_t publication::offer(py::object data)
{
    if (py::isinstance<py::buffer>(data))
    {
        py::buffer buffer = data;
        py::buffer_info info = buffer.request(false);

        AtomicBuffer aeron_buffer(reinterpret_cast<uint8_t*>(info.ptr), info.size * info.itemsize);
        return aeron_publication_->offer(aeron_buffer);
    }
    else if (py::isinstance<py::str>(data))
    {
        auto characters = data.cast<std::string>();

        AtomicBuffer aeron_buffer(
                const_cast<uint8_t*>(
                        reinterpret_cast<const uint8_t*>(characters.data())), characters.length());
        return aeron_publication_->offer(aeron_buffer);
    }

    throw py::type_error("unsupported data type!");
}

void publication::close()
{
    aeron_publication_->close();
}

bool publication::__bool__() const
{
    return aeron_publication_ && aeron_publication_->isConnected();
}

string publication::__str__() const
{
    return format("publication: channel:[{}] stream_id:[{}] session_id:[{}]",
            aeron_publication_->channel(),
            aeron_publication_->streamId(),
            aeron_publication_->sessionId());
}

PYBIND11_MODULE(_publication, m)
{
    py::class_<publication>(m, "Publication")
            .def_property_readonly("channel", &publication::channel)
            .def_property_readonly("stream_id", &publication::stream_id)
            .def_property_readonly("session_id", &publication::session_id)
            .def_property_readonly("is_connected", &publication::is_connected)
            .def_property_readonly("is_closed", &publication::is_closed)
            .def_property_readonly("is_original", &publication::is_original)
            .def("offer", &publication::offer,
                    py::arg("data"))
            .def("close", &publication::close)
            .def("__bool__", &publication::__bool__)
            .def("__str__", &publication::__str__);

}

