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

#include "_subscription.hpp"

#include <Aeron.h>
#include <AeronArchive.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>


/**
 * @brief Provides an interop proxy for interaction with Aeron archive.
 */
class archive final
{
public:
    /**
     * @brief Creates an instance of **archive** for the .
     * @param channel Channel to find the latest reply for.
     * @param stream_id Stream id to find the latest reply for.
     * @param args Additional options for the archive.
     */
    explicit archive(const std::string& channel, int32_t stream_id, pybind11::kwargs args);
    /**
     * @brief
     * @param recording_id
     * @param args
     */
    explicit archive(int64_t recording_id, pybind11::kwargs args);

    /**
     * @brief Creates an archive from a recording id.
     * @param recording_id An identifier of recording to locate.
     * @param args Additional arguments passed during creation of an archive client.
     * @return An instance of archive client proxy.
     */
    static archive from_recording_id(int64_t recording_id, pybind11::kwargs args);

    /**
     * @brief Initites replay from a stream.
     * @param channel Channel to reply on.
     * @param stream_id Stream to reply on.
     * @param position Position to start reply from.
     * @return An instance of subscription representing replay data
     */
    subscription replay(const std::string& channel, int32_t stream_id, int64_t position);

private:
    archive(pybind11::kwargs& args);

    int64_t find_latest_recording_id(const std::string& channel, int32_t streamId);

    int64_t recording_id_;
    std::shared_ptr<aeron::archive::AeronArchive> aeron_archive_;

};


