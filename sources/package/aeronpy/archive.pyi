# Copyright 2018 Fairtide Pte. Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from typing import NoReturn, Optional
from .exclusive_publication import ExclusivePublication
from .publication import Publication
from .subscription import Subscription


class Recording:
    id = ... # type: int
    position = ... # type: int

    def replay(self, channel: str, stream_id: int, position: int = 0) -> Subscription: ...
    def truncate(self, position: int) -> NoReturn: ...


class Archive:
    def __init__(
            config_file: str=None,
            aeron_dir: str=None,
            message_timeout_ns: int=None,
            control_request_channel: str=None,
            control_request_stream_id: int=None,
            control_response_channel: str=None,
            control_response_stream_id: int=None,
            recording_events_channel: str=None,
            recording_events_stream_id: int=None,
            control_term_buffer_sparse: bool=None,
            control_term_buffer_length: int=None,
            control_mtu_length: int=None) -> NoReturn: ...

    def find(self, recording_id: int) -> Optional[Recording]: ...
    def find_last(self, channel: str, stream_id: int) -> Optional[Recording]: ...

    def add_recorded_publication(self, channel: str, stream_id: int) -> Publication: ...
    def add_recorded_exclusive_publication(self, channel: str, stream_id: int) -> ExclusivePublication: ...
