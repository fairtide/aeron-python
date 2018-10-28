from typing import Union, NoReturn


class Publication:
    channel = ... # type: str
    stream_id = ... # type: int
    session_id = ... # type: int

    is_connected = ... # type: bool
    is_closed = ... # type: bool
    is_original = ... # type: bool


    def offer(self, data: Union[bytearray, str]) -> NoReturn: ...
    def close(self) -> NoReturn: ...

    def __bool__(self) -> bool: ...
    def __str__(self)-> str: ...