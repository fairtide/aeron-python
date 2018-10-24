#include "_subscription.hpp"

#include <pybind11/pybind11.h>

using namespace std;
using namespace std::chrono;
using namespace aeron;
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

int subscription::poll(py::function handler, int fragment_limit)
{
    return aeron_subscription_->poll(
            [&](const AtomicBuffer& buffer, util::index_t offset, util::index_t length, const Header& header)
            {
                py::gil_scoped_acquire gil_guard;

                auto data_info = py::buffer_info(
                        buffer.buffer() + offset,
                        sizeof(uint8_t),
                        py::format_descriptor<uint8_t>::format(),
                        length);

                handler(py::memoryview(data_info));

            }, fragment_limit);
}

bool subscription::__bool__() const
{
    return aeron_subscription_ && aeron_subscription_->isConnected();
}

PYBIND11_MODULE(_subscription, m)
{
    static constexpr auto default_fragment_limit = 10;

    py::class_<subscription>(m, "Subscription")
            .def_property_readonly("channel", &subscription::channel)
            .def_property_readonly("stream_id", &subscription::stream_id)
            .def("poll", &subscription::poll,
                    py::arg("handler"),
                    py::arg("fragment_limit") = default_fragment_limit,
                    py::call_guard<py::gil_scoped_release>())
            .def("__bool__", &subscription::__bool__);

}




