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


/**
 * @brief Represents interop proxy for Aeron publication.
 */
class publication final
{
public:
    /**
     * @brief Creates proxy object for given Aeron publication.
     * @param aeron_publication A pointer to base Aeron publication.
     */
    publication(std::shared_ptr<aeron::Publication> aeron_publication);

    /**
     * @brief Gets channel name this publisher is for.
     * @return Channel name.
     */
    const std::string& channel() const;
    /**
     * @brief Gets stream id this publisher is for.
     * @return Stream id.
     */
    int64_t stream_id() const;
    /**
     * @brief Gets session id this publisher is for.
     * @return Session id.
     */
    int32_t session_id() const;
    /**
     * @brief
     * @return
     */
    int32_t initial_term_id() const;
    /**
     * @brief Indicates if publication is connected to media driver.
     * @return True if connected to media driver, False otherwise.
     */
    bool is_connected() const;
    /**
     * @brief Indicates if the underlying publication is closed.
     * @return True if closed, False otherwise.
     */
    bool is_closed() const;
    /**
     * @brief Indicates if the publication is original publication.
     * @return True if publication is original, false otherwise.
     */
    bool is_original() const;

    /**
     * Offers a data block to open stream this publisher is for.
     * @param data Data block to be offered to the stream.
     * @return Number of bytes sent or BACK_PRESSURED or NOT_CONNECTED.
     */
    int64_t offer(pybind11::object data);
    /**
     * @brief Closes the underlying publication.
     */
    void close();

    /**
     * @brief Checks state of the publisher.
     * @return True if publisher is connected and is ready to offer data.
     */
    bool __bool__() const;
    /**
     * @brief Provides debug description of publication.
     * @return Debug description of publication.
     */
    std::string __str__() const;

private:
    std::shared_ptr<aeron::Publication> aeron_publication_;

};
