from typing import Callable


class Subscription:
    channel = ... # type: str
    stream_id = ... # type: int

    def poll(self, handler: Callable, fragment_limit: int) -> int: ...

