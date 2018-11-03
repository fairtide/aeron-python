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

#include "_exclusive_publication.hpp"
#include "_publication.hpp"
#include "_subscription.hpp"

#include <Aeron.h>
#include <pybind11/pybind11.h>

#include <list>
#include <memory>
#include <string>


/**
 * @brief Provides a helper for exposing Aeron to python.
 */
class context final
{
public:
    /**
     * @brief Creates a new instance of Aeron interop context.
     * @details
     * This constructor creates and configures underlying Aeron client. Configuration options can be passed
     * through kwargs.
     */
    explicit context(pybind11::kwargs args);

    /**
     * @brief Adds and waits for initialisation of a subscription.
     * @param channel Subscribed channel identifier.
     * @param stream_id Subscribed stream number.
     * @return An interop proxy for added subscription.
     */
    subscription add_subscription(const std::string& channel, int32_t stream_id);
    /**
     * @brief Adds and waits for initialisation of a publication.
     * @param channel Channel this publication should be on.
     * @param stream_id Id of stream this publication should be on.
     * @return An interop proxy for added publication.
     */
    publication add_publication(const std::string& channel, int32_t stream_id);
    /**
     * @brief Adds and waits for initialisation of an exclusive publication.
     * @details
     * Added publication will have unique session.
     * @param channel Channel the publication should be on.
     * @param stream_id Id of stream this publication should be on.
     * @return An interop proxy for added publication.
     */
    exclusive_publication add_exclusive_publication(const std::string& channel, int32_t stream_id);

private:
    aeron::Context aeron_context_;
    std::shared_ptr<aeron::Aeron> aeron_instance_;

};