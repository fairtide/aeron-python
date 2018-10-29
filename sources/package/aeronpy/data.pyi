from typing import NoReturn


class Image:
    session_id = ... # type: int

    is_end_of_stream = ... # type: bool
    is_closed = ... # type: bool

    def close(self) -> NoReturn: ...


class Header:
    stream_id = ... # type: int
    session_id = ... # type: int

    def __str__(self) -> str: ...


NOT_CONNECTED = ... # type: int
BACK_PRESSURED = ... # type: int
ADMIN_ACTION = ... # type: int
PUBLICATION_CLOSED = ... # type: int
MAX_POSITION_EXCEEDED = ... # type: int