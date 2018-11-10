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
#include <AeronArchive.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>


/**
 * @brief Represents existing Aeron recording.
 */
class recording final
{
public:
    /**
     * @brief Creates an instance of a **recording** with given id.
     * @param aeron_archive An instance of archive this recording belongs to.
     * @param id Recording id.
     */
    explicit recording(std::shared_ptr<aeron::archive::AeronArchive> aeron_archive, int64_t id);

    /**
     * @brief Gets id of the recording.
     * @return Id of the recording.
     */
    int64_t id() const;
    /**
     * @brief Gets current position in the recording.
     * @return Current position in the recording.
     */
    int64_t position() const;

    /**
     * @brief Initiates replay from the stream.
     * @details
     * Depending on archive configuration this will initiate replay from the local or remote host. Returned subscription
     * allows consumption of replayed stream. The stream will be delivered on the given **channel** and **stream**.
     *
     * @param channel Channel to replay recorded stream on.
     * @param stream_id Stream to replay the recording as.
     * @param position Position within the recording to transmi the first replay message from.
     * @return A subscription allowing consumption of replayed stream.
     */
    subscription replay(const std::string& channel, int32_t stream_id, int64_t position);
    /**
     * @brief Truncates the recording up to the given position.
     * @details
     * This removes all messages before given position from the recording.
     *
     * @param position The first position in the modified recording.
     */
    void truncate(int64_t position);

    /**
     * @brief Provides debug description of the recording.
     * @return Debug description of the recording.
     */
    std::string __str__() const;

private:
    std::shared_ptr<aeron::archive::AeronArchive> aeron_archive_;
    int64_t id_;

};

/**
 * @brief Provides an interop proxy for interaction with Aeron archive.
 * @details
 * The aeron-archive is an service which enables Aeron data stream recording and replay support from an archive.
 */
class archive final
{
public:
    /**
     * @brief Creates an instance of an **archive**.
     * @details
     * This creates an instance of an **archive** configured according to passed options. Configured archive allows
     * accessing existing recordings and initiating new ones.
     *
     * @param args Configuration options for this archive.
     */
    explicit archive(pybind11::kwargs args);

    /**
     * @brief Attempts to get recording associated with the specified id.
     * @param recording_id The id of recording to look for.
     * @return On success, recording associated to the id. On failure, None.
     */
    std::unique_ptr<recording> find(int64_t recording_id);
    /**
     * @brief Attempts to find last recording for given channel and stream.
     * @param channel Channel for which
     * @param stream_id Stream to reply on.
     * @param position Position to start reply from.
     * @return On success, latest recording for given channel and stream. On failure, None.
     */
    std::unique_ptr<recording> find_last(const std::string& channel, int32_t stream_id);

    /**
     * @brief Adds a publication with automatic archive recording.
     * @details
     * This creates a publication in a similar way to **context::add_publication** wit automatic registration of
     * stream recording. Multiple publications share the same session.
     *
     * @param channel Channel to open the publication on.
     * @param stream_id Stream to open the publication on.
     * @return An instance of publisher through which data can be offered to the stream.
     */
    publication add_recorded_publication(const std::string& channel, int32_t stream_id);
    /**
     * @brief Adds an exclusive publication with automatic recording.
     * @details
     * This method behaves indetically to **context::add_exclusive_publication**, with addional registration
     * of stream recording. Each publication will have its unique session.
     *
     * @param channel Channel to open the publication on.
     * @param stream_id Stream to open the publication on.
     * @return An instance of publisher through which data can be offered to the stream.
     */
    exclusive_publication add_recorded_exclusive_publication(const std::string& channel, int32_t stream_id);

    /**
     * @brief Provides debug description of the archive.
     * @return Debug description of the archive.
     */
    std::string __str__() const;

private:
    int64_t find_latest_recording_id(const std::string& channel, int32_t streamId);

    int64_t recording_id_;
    std::shared_ptr<aeron::archive::AeronArchive> aeron_archive_;

};


