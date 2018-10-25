#include "_context.hpp"

#include <string>

using namespace std;
using namespace aeron;
namespace py = pybind11;


context::context(py::kwargs args)
{
    static constexpr auto aeron_dir_key = "aeron_dir";

    aeron::Context aeron_context;

    if (args.contains(aeron_dir_key))
    {
        auto aeron_dir = args[aeron_dir_key].cast<string>();
        aeron_context.aeronDir(aeron_dir);
    }

    aeron_instance_ = Aeron::connect(aeron_context);
}

subscription context::add_subscription(const string& channel, int32_t stream_id)
{
    auto id = aeron_instance_->addSubscription(channel, stream_id);
    auto subscription = aeron_instance_->findSubscription(id);

    // wait for the subscription to be valid
    while (!subscription)
    {
        std::this_thread::yield();
        subscription = aeron_instance_->findSubscription(id);
    }

    return subscription;
}

publication context::add_publication(const string& channel, int32_t stream_id)
{
    auto id = aeron_instance_->addPublication(channel, stream_id);
    auto publication = aeron_instance_->findPublication(id);

    // wait for the subscription to be valid
    while (!publication)
    {
        std::this_thread::yield();
        publication = aeron_instance_->findPublication(id);
    }

    return publication;
}

PYBIND11_MODULE(_context, m)
{
    py::class_<context>(m, "Context")
            .def(py::init<py::kwargs>())
            .def("add_subscription", &context::add_subscription,
                    py::arg("channel"),
                    py::arg("stream_id"),
                    py::call_guard<py::gil_scoped_release>(),
                    py::keep_alive<0, 1>())
            .def("add_publication", &context::add_publication,
                    py::arg("channel"),
                    py::arg("stream_id"),
                    py::call_guard<py::gil_scoped_release>(),
                    py::keep_alive<0, 1>());

}

