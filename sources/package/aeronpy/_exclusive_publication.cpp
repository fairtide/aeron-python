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

#include "_exclusive_publication.hpp"

#include <fmt/format.h>

using namespace std;
using namespace aeron;
using namespace fmt;
namespace py = pybind11;


exclusive_publication::exclusive_publication(shared_ptr<ExclusivePublication> aeron_exclusive_publication)
    :
        aeron_exclusive_publication_(aeron_exclusive_publication)
{

}

const string& exclusive_publication::channel() const
{
    return aeron_exclusive_publication_->channel();
}

int64_t exclusive_publication::stream_id() const
{
    return aeron_exclusive_publication_->streamId();
}

int32_t exclusive_publication::session_id() const
{
    return aeron_exclusive_publication_->sessionId();
}

int32_t exclusive_publication::initial_term_id() const
{
    return aeron_exclusive_publication_->initialTermId();
}

bool exclusive_publication::is_connected() const
{
    return aeron_exclusive_publication_->isConnected();
}

bool exclusive_publication::is_closed() const
{
    return aeron_exclusive_publication_->isClosed();
}

bool exclusive_publication::is_original() const
{
    return aeron_exclusive_publication_->isOriginal();
}

int64_t exclusive_publication::offer(py::object data)
{
    if (py::isinstance<py::buffer>(data))
    {
        py::buffer buffer = data;
        py::buffer_info info = buffer.request(false);

        AtomicBuffer aeron_buffer(reinterpret_cast<uint8_t*>(info.ptr), info.size * info.itemsize);
        return aeron_exclusive_publication_->offer(aeron_buffer);
    }
    else if (py::isinstance<py::str>(data))
    {
        auto characters = data.cast<std::string>();

        AtomicBuffer aeron_buffer(
                const_cast<uint8_t*>(
                        reinterpret_cast<const uint8_t*>(characters.data())), characters.length());
        return aeron_exclusive_publication_->offer(aeron_buffer);
    }

    throw py::type_error("unsupported data type!");
}

void exclusive_publication::close()
{
    aeron_exclusive_publication_->close();
}

bool exclusive_publication::__bool__() const
{
    return aeron_exclusive_publication_ && aeron_exclusive_publication_->isConnected();
}

string exclusive_publication::__str__() const
{
    return format("exclusive publication: hannel:[{}] stream_id:[{}] session_id:[{}]",
            aeron_exclusive_publication_->channel(),
            aeron_exclusive_publication_->streamId(),
            aeron_exclusive_publication_->sessionId());
}

PYBIND11_MODULE(_exclusive_publication, m)
{
    py::class_<exclusive_publication>(m, "ExclusivePublication")
            .def_property_readonly("channel", &exclusive_publication::channel)
            .def_property_readonly("stream_id", &exclusive_publication::stream_id)
            .def_property_readonly("session_id", &exclusive_publication::session_id)
            .def_property_readonly("is_connected", &exclusive_publication::is_connected)
            .def_property_readonly("is_closed", &exclusive_publication::is_closed)
            .def_property_readonly("is_original", &exclusive_publication::is_original)
            .def("offer", &exclusive_publication::offer,
                 py::arg("data"))
            .def("close", &exclusive_publication::close)
            .def("__bool__", &exclusive_publication::__bool__)
            .def("__str__", &exclusive_publication::__str__);

}

