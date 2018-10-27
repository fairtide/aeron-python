from typing import NoReturn
from .subscription import Subscription


class Archive:
    def __init__(self, channel: str, stream_id: int, config_file: str=None) -> NoReturn: ...

    def replay(self, channel: str, stream_id: int, position: int = 0) -> Subscription: ...