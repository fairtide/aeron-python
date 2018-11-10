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


/**
 * @brief Represents interop proxy for Aeron exclusive publication.
 */
class exclusive_publication final
{
public:
    /**
     * @brief
     * @param aeron_exclusive_publication
     */
    exclusive_publication(std::shared_ptr<aeron::ExclusivePublication> aeron_exclusive_publication);

    /**
     * @brief
     * @return
     */
    const std::string& channel() const;
    /**
     * @brief
     * @return
     */
    int64_t stream_id() const;
    /**
     *
     * @return
     */
    int32_t session_id() const;
    /**
     *
     * @return
     */
    int32_t initial_term_id() const;
    /**
     * @brief
     * @return
     */
    bool is_connected() const;
    /**
     * @brief
     * @return
     */
    bool is_closed() const;
    /**
     * @brief
     * @return
     */
    bool is_original() const;

    /**
     * Offers a data block to open stream this publisher is for.
     * @param data Data block to be offered to the stream.
     * @return Number of bytes sent or BACK_PRESSURED or NOT_CONNECTED.
     */
    int64_t offer(pybind11::object data);

    /**
     * @brief
     */
    void close();

    /**
     * @brief
     * @return
     */
    bool __bool__() const;
    /**
     * @brief
     * @return
     */
    std::string __str__() const;

private:
    std::shared_ptr<aeron::ExclusivePublication> aeron_exclusive_publication_;

};
