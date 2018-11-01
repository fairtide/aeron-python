from typing import Union, NoReturn


class ExclusivePublication:
    channel = ... # type: str
    stream_id = ... # type: int

    def offer(self, data: Union[bytearray, str]) -> NoReturn: ...
    def close(self) -> NoReturn: ...

    def __bool__(self) -> bool: ...