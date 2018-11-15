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

from typing import Callable, List, NoReturn, Union
from .data import Header, Image


class Subscription:
    channel = ... # type: str
    stream_id = ... # type: int

    is_connected = ... # type: bool
    is_closed = ... # type: bool

    images = ... # type: List[Image]

    def poll(self, handler: Union[Callable[[memoryview], NoReturn], Callable[[memoryview, Header], NoReturn]], fragment_limit: int=10) -> int: ...
    def poll_eos(self) -> int: ...

