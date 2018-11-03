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

#include <string>


/**
 * @brief Helper for interop with Aeron Image type.
 */
class image final
{
public:
    /**
     * @brief
     * @param self
     * @return
     */
    static int32_t session_id(aeron::Image& self);
    /**
     * @brief
     * @param self
     * @return
     */
    static bool is_end_of_stream(aeron::Image& self);
    /**
     * @brief
     * @param self
     * @return
     */
    static bool is_closed(aeron::Image& self);
    /**
     * @brief
     */
    static void close(aeron::Image& self);

    /**
     * @brief
     * @return
     */
    static std::string __str__(aeron::Image& self);

};

/**
 * @brief A helper for interop with Aeron Header type.
 */
class header final
{
public:
    /**
     * @brief
     * @param self
     * @return
     */
    static int32_t stream_id(aeron::Header& self);
    /**
     * @brief
     * @return
     */
    static int32_t session_id(aeron::Header& self);

    /**
     * @brief
     * @param self
     * @return
     */
    static std::string __str__(aeron::Header& self);

};
