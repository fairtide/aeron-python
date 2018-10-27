from datetime import timedelta
from typing import Callable, NoReturn
from .publication import Publication
from .subscription import Subscription


class Context:
    def __init__(self,
                 aeron_dir: str=None,
                 media_driver_timeout: timedelta=None,
                 resource_linger_timeout: timedelta=None,
                 user_conductor_agent_invoker: bool=False,
                 error_handler: Callable[[Exception], NoReturn]=None,
                 new_publication_handler: Callable[[str, int, int, int], NoReturn]=None,
                 **kwargs) -> NoReturn: ...

    def add_subscription(self, channel: str, stream_id: int) -> Subscription: ...
    def add_publication(self, channel: str, stream_id: int) -> Publication: ...