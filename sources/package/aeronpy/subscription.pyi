from typing import Callable, NoReturn


class Subscription:
    channel = ... # type: str
    stream_id = ... # type: int

    is_connected = ... # type: bool
    is_closed = ... # type: bool

    def poll(self, handler: Callable[[memoryview], NoReturn], fragment_limit: int=10) -> int: ...
    def poll_eos(self) -> int: ...

