from typing import Callable, NoReturn


class Subscription:
    channel = ... # type: str
    stream_id = ... # type: int

    def poll(self, handler: Callable[[memoryview], NoReturn], fragment_limit: int) -> int: ...

