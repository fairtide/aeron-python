#include "_context.hpp"

#include <pybind11/chrono.h>

#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;
using namespace aeron;
namespace py = pybind11;


context::context(py::kwargs args)
{
    static constexpr auto aeron_dir_key = "aeron_dir";
    static constexpr auto media_driver_timeout_key = "media_driver_timeout";
    static constexpr auto resource_linger_timeout_key = "resource_linger_timeout";
    static constexpr auto use_conductor_agent_invoker_key = "user_conductor_agent_invoker";

    static constexpr auto error_handler_key = "error_handler";
    static constexpr auto new_publication_handler_key = "new_publication_handler";
    static constexpr auto new_exclusive_publication_handler_key = "new_exclusive_publication_handler";
    static constexpr auto new_subscription_handler_key = "new_subscription_handler";
    static constexpr auto available_image_handler_key = "available_image_handler";
    static constexpr auto unavailable_image_handler_key = "unavailable_image_handler";

    // context properties
    if (args.contains(aeron_dir_key))
    {
        auto aeron_dir = args[aeron_dir_key].cast<string>();
        aeron_context_.aeronDir(aeron_dir);
    }
    if (args.contains(media_driver_timeout_key))
    {
        auto timeout = args[media_driver_timeout_key].cast<milliseconds>();
        aeron_context_.mediaDriverTimeout(timeout.count());
    }
    if (args.contains(resource_linger_timeout_key))
    {
        auto timeout = args[resource_linger_timeout_key].cast<milliseconds>();
        aeron_context_.resourceLingerTimeout(timeout.count());
    }
    if (args.contains(use_conductor_agent_invoker_key))
    {
        auto use_conductor_agent_invoker = args[use_conductor_agent_invoker_key].cast<bool>();
        aeron_context_.useConductorAgentInvoker(use_conductor_agent_invoker);
    }

    // callbacks
    if (args.contains(error_handler_key))
    {
        auto handler = args[error_handler_key].cast<py::function>();
        aeron_context_.errorHandler(handler);
    }
    if (args.contains(new_publication_handler_key))
    {
        auto handler = args[new_publication_handler_key].cast<py::function>();
        aeron_context_.newPublicationHandler(handler);
    }

    aeron_instance_ = Aeron::connect(aeron_context_);
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

