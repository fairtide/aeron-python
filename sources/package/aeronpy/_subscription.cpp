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

#include <functional>
#include <fmt/format.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

using namespace std;
using namespace std::chrono;
using namespace aeron;
using namespace fmt;
namespace py = pybind11;

/*
const std::function<aeron::ControlledPollAction(aeron::concurrent::AtomicBuffer&, int, int, aeron::concurrent::logbuffer::Header&)>&
subscription::subscription(std::shared_ptr<aeron::Subscription>)::<lambda(aeron::concurrent::AtomicBuffer&, aeron::util::index_t, aeron::util::index_t, aeron::concurrent::logbuffer::Header&)>

 */
subscription::subscription(shared_ptr<Subscription> aeron_subscription)
    : aeron_subscription_(aeron_subscription)
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

void subscription::init_fragment_assembler()
{
    if (!fragmentAssembler_)
    {
        fragmentAssembler_ = make_unique<ControlledFragmentAssembler>(
            [this](AtomicBuffer& buffer,
                   index_t offset,
                   index_t length,
                   Header& header)
            {
                py::gil_scoped_acquire gil_guard;

                auto data_info = py::buffer_info(
                    buffer.buffer() + offset,
                    sizeof(uint8_t),
                    pybind11::format_descriptor<uint8_t>::format(),
                    length);

                pybind11::memoryview memview = py::memoryview(data_info);

                py_func_handler(memview);

                return ControlledPollAction::CONTINUE;
            });
    }
}

int subscription::poll(py::function handler, int fragment_limit)
{
    py_func_handler = handler;

    init_fragment_assembler();

    return aeron_subscription_->poll(fragmentAssembler_->handler(), fragment_limit);
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
            .def("__bool__", &subscription::__bool__)
            .def("__str__", &subscription::__str__);

}
