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

#include "_archive.hpp"

#include <ChannelUri.h>
#include <Configuration.h>

#include <cstdlib>

using namespace std;
namespace py = pybind11;


archive::archive(const string& channel, int32_t stream_id, py::kwargs args)
    :
        archive(args)
{
    recording_id_ = find_latest_recording_id(channel, stream_id);
}

archive::archive(int64_t recording_id, pybind11::kwargs args)
    :
        archive(args)
{
    recording_id_ = recording_id;
}

archive archive::from_recording_id(int64_t recording_id, py::kwargs args)
{
    return archive(recording_id, args);
}

subscription archive::replay(const string& channel, int32_t stream_id, int64_t position_id)
{
    auto subscription = aeron_archive_->replay(
            recording_id_,
            position_id,
            numeric_limits<std::int64_t>::max(),
            channel,
            stream_id);

    return subscription;
}

archive::archive(pybind11::kwargs& args) 
{
    static constexpr auto config_file_key = "config_file";
    static constexpr auto aeron_dir_key = "aeron_dir";

    static constexpr auto default_aeron_dir_var = "AERON_DIR";

    // aeron archive context initialisation
    unique_ptr<aeron::archive::Context> aeron_archive_context;
    if (args.contains(config_file_key))
    {
        auto config_file = args[config_file_key].cast<string>();
        aeron::archive::Configuration configuration(config_file);

        aeron_archive_context = make_unique<aeron::archive::Context>(configuration);
    }
    else
    {
        aeron_archive_context = make_unique<aeron::archive::Context>();
    }

    // defaults from environment variables
    if(auto default_aeron_dir = getenv(default_aeron_dir_var))
        aeron_archive_context->aeronDirectoryName(default_aeron_dir);

    // context properties
    if (args.contains(aeron_dir_key))
    {
        auto aeron_dir = args[aeron_dir_key].cast<string>();
        aeron_archive_context->aeronDirectoryName(aeron_dir);
    }

    aeron_archive_ = aeron::archive::AeronArchive::connect(*aeron_archive_context);
}

int64_t archive::find_latest_recording_id(const string& channel, int32_t streamId)
{
    std::int64_t lastRecordingId{-1};

    auto consumer = [&](
            long controlSessionId,
            long correlationId,
            long recordingId,
            long startTimestamp,
            long stopTimestamp,
            long startPosition,
            long stopPosition,
            int initialTermId,
            int segmentFileLength,
            int termBufferLength,
            int mtuLength,
            int sessionId,
            int streamId,
            const string& strippedChannel,
            const string& originalChannel,
            const string& sourceIdentity)
    {
        lastRecordingId = recordingId;
    };

    std::int32_t foundCount = aeron_archive_->listRecordingsForUri(
            0,
            numeric_limits<std::int32_t>::max(),
            channel,
            streamId,
            consumer);
    return lastRecordingId;
}

PYBIND11_MODULE(_archive, m)
{
    static constexpr auto default_position = 0;

    py::class_<archive>(m, "Archive")
            .def(py::init<const string&, int32_t, py::kwargs>(),
                    py::arg("channel"),
                    py::arg("stream"))
            .def_static("from_recording_id", &archive::from_recording_id,
                    py::arg("recording_id"))
            .def("replay", &archive::replay,
                    py::arg("channel"),
                    py::arg("stream_id"),
                    py::arg("position") = default_position,
                    py::call_guard<py::gil_scoped_release>(),
                    py::keep_alive<0, 1>());
}

