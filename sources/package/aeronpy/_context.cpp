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

#include "_context.hpp"

#include <fmt/format.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

#include <chrono>
#include <cstdlib>
#include <string>

using namespace std;
using namespace std::chrono;
using namespace aeron;
using namespace fmt;
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

    static constexpr auto default_aeron_dir_var = "AERON_DIR";

    // defaults from environment variables
    if(auto default_aeron_dir = getenv(default_aeron_dir_var))
        aeron_context_.aeronDir(default_aeron_dir);

    // context properties
    if (args.contains(aeron_dir_key))
    {
        if (!args[aeron_dir_key].is_none())
        {
            auto aeron_dir = args[aeron_dir_key].cast<string>();
            aeron_context_.aeronDir(aeron_dir);
        }
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
        auto handler = args[error_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", error_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.errorHandler([handler = handler.cast<py::function>()](auto& error)
        {
            py::gil_scoped_acquire gil_guard;
            handler(error);
        });
    }
    if (args.contains(new_publication_handler_key))
    {
        auto handler = args[new_publication_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", new_publication_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.newPublicationHandler(
                [handler = handler.cast<py::function>()](
                        auto& channel, auto stream_id, auto session_id, auto correlation_id)
                {
                    py::gil_scoped_acquire gil_guard;
                    handler(channel, stream_id, session_id, correlation_id);
                });
    }
    if (args.contains(new_exclusive_publication_handler_key))
    {
        auto handler = args[new_exclusive_publication_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", new_exclusive_publication_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.newExclusivePublicationHandler(
                [handler = handler.cast<py::function>()](
                        auto& channel, auto stream_id, auto session_id, auto correlation_id)
                {
                    py::gil_scoped_acquire gil_guard;
                    handler(channel, stream_id, session_id, correlation_id);
                });
    }
    if (args.contains(new_subscription_handler_key))
    {
        auto handler = args[new_subscription_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", new_subscription_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.newSubscriptionHandler(
                [handler = handler.cast<py::function>()](
                        auto &channel, auto stream_id, auto correlation_id)
                {
                    py::gil_scoped_acquire gil_guard;
                    handler(channel, stream_id, correlation_id);
                });
    }
    if (args.contains(available_image_handler_key))
    {
        auto handler = args[available_image_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", available_image_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.availableImageHandler(
                [handler = handler.cast<py::function>()](auto& image)
                {
                    py::gil_scoped_acquire gil_guard;
                    handler(image);
                });
    }
    if (args.contains(unavailable_image_handler_key))
    {
        auto handler = args[unavailable_image_handler_key];
        if (!py::isinstance<py::function>(handler))
        {
            auto reason = format("{} has to be a function!", unavailable_image_handler_key);
            throw py::type_error(reason);
        }

        aeron_context_.unavailableImageHandler(
                [handler = handler.cast<py::function>()](auto& image)
                {
                    py::gil_scoped_acquire gil_guard;
                    handler(image);
                });
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

exclusive_publication context::add_exclusive_publication(const std::string &channel, int32_t stream_id)
{
    auto id = aeron_instance_->addExclusivePublication(channel, stream_id);
    auto publication = aeron_instance_->findExclusivePublication(id);

    // wait for the subscription to be valid
    while (!publication)
    {
        std::this_thread::yield();
        publication = aeron_instance_->findExclusivePublication(id);
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
                    py::keep_alive<0, 1>())
            .def("add_exclusive_publication", &context::add_exclusive_publication,
                 py::arg("channel"),
                 py::arg("stream_id"),
                 py::call_guard<py::gil_scoped_release>(),
                 py::keep_alive<0, 1>());

}

