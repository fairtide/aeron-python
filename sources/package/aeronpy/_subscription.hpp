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

#pragma once

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>
#include <vector>


/**
 * @brief Represents an interop proxy for an Aeron subscription.
 */
class subscription final
{
public:
    /**
     * @brief Creates a new instance of proxy wrapping given aeron subscription.
     * @param aeron_subscription Aeron subscription to wrap.
     */
    subscription(std::shared_ptr<aeron::Subscription> aeron_subscription);

    /**
     * @brief Gets channel definition this subscription is for.
     * @return Channel definition this subscription is for.
     */
    const std::string& channel() const;
    /**
     * @brief Gets stream identifier this subscription is for.
     * @return Stream identifier this subscription is for.
     */
    int64_t stream_id() const;
    /**
     * @brief Indicates if subscription is connected to publisher or not.
     * @return True if connected, false otherwise.
     */
    bool is_connected() const;
    /**
     * @brief
     * @return
     */
    bool is_closed() const;

    /**
     * @brief Gets a list of images this subscription currently handles.
     * @details
     * Each image represents a different session within the channel/stream.
     *
     * @return A list of images.
     */
    std::vector<aeron::Image> images() const;

    /**
     * @brief
     * @param handler
     * @param fragment_limit
     * @return
     */
    int poll(pybind11::function handler, int fragment_limit);
    /**
     * @brief
     * @return
     */
    int poll_eos(pybind11::object handler);

    /**
     * @brief
     * @return
     */
    bool __bool__() const;
    /**
     * @brief Produces debug, textual representation of this subscription.
     * @return Debug, textual representation of this subscription.
     */
    std::string __str__() const;

private:
    bool is_complete_poll_handler(pybind11::function& handler);

    std::shared_ptr<aeron::Subscription> aeron_subscription_;

};
