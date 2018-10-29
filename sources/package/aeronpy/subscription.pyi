from typing import Callable, List, NoReturn, Union
from .data import


class Subscription:
    channel = ... # type: str
    stream_id = ... # type: int

    is_connected = ... # type: bool
    is_closed = ... # type: bool

    images = ... # type: List[Image]

    def poll(self, handler: Union[Callable[[memoryview], NoReturn], Callable[[memoryview, Header], NoReturn]], fragment_limit: int=10) -> int: ...
    def poll_eos(self) -> int: ...

