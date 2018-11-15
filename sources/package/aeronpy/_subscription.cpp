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

#include "_subscription.hpp"

#include <fmt/format.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

using namespace std;
using namespace std::chrono;
using namespace aeron;
using namespace fmt;
namespace py = pybind11;


subscription::subscription(shared_ptr<Subscription> aeron_subscription)
    :
        aeron_subscription_(aeron_subscription)
{

}

const string& subscription::channel() const
{
    return aeron_subscription_->channel();
}

int64_t subscription::stream_id() const
{
    return aeron_subscription_->streamId();
}

bool subscription::is_connected() const
{
    return aeron_subscription_->isConnected();
}

bool subscription::is_closed() const
{
    return aeron_subscription_->isClosed();
}

vector<Image> subscription::images() const
{

    return *aeron_subscription_->images();
}

int subscription::poll(py::function handler, int fragment_limit)
{
    return aeron_subscription_->poll(
            [&, this](auto& buffer, auto offset, auto length, auto& header)
            {
                py::gil_scoped_acquire gil_guard;

                auto data_info = py::buffer_info(
                        buffer.buffer() + offset,
                        sizeof(uint8_t),
                        py::format_descriptor<uint8_t>::format(),
                        length);

                if (is_complete_poll_handler(handler)) // expected performance hit
                    handler(py::memoryview(data_info), header);
                else
                    handler(py::memoryview(data_info));
            },
            fragment_limit);
}

int subscription::poll_eos(py::object handler)
{
    return aeron_subscription_->pollEndOfStreams([&](auto& image)
    {
        py::gil_scoped_acquire gil_guard;

        if (handler)
            handler(image);
    });
}

bool subscription::__bool__() const
{
    return aeron_subscription_ && aeron_subscription_->isConnected();
}

string subscription::__str__() const
{
    return format("subscription: channel:[{}] stream:[{}]",
            aeron_subscription_->channel(),
            aeron_subscription_->streamId());
}

bool subscription::is_complete_poll_handler(py::function& handler)
{
    static constexpr auto inspect_module = "inspect";
    static constexpr auto signature_attribute = "signature";
    static constexpr auto parameters_attribute = "parameters";

    auto inspect = py::module::import(inspect_module);
    auto signature_func = inspect.attr(signature_attribute);
    auto signature = signature_func(handler);

    return py::len(signature.attr(parameters_attribute)) > 1u;
}

PYBIND11_MODULE(_subscription, m)
{
    static constexpr auto default_fragment_limit = 10;

    py::class_<subscription>(m, "Subscription")
            .def_property_readonly("channel", &subscription::channel)
            .def_property_readonly("stream_id", &subscription::stream_id)
            .def_property_readonly("is_connected", &subscription::is_connected)
            .def_property_readonly("is_closed", &subscription::is_closed)
            .def_property_readonly("images", &subscription::images)
            .def("poll", &subscription::poll,
                    py::arg("handler"),
                    py::arg("fragment_limit") = default_fragment_limit,
                    py::call_guard<py::gil_scoped_release>())
            .def("poll_eos", &subscription::poll_eos,
                    py::arg("handler") = py::none(),
                    py::call_guard<py::gil_scoped_release>())
            .def("__bool__", &subscription::__bool__)
            .def("__str__", &subscription::__str__);

}




