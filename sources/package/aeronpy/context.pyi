from datetime import timedelta
from typing import Callable, NoReturn
from .data import Image
from .exclusive_publication import ExclusivePublication
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
                 new_exclusive_publication_handler: Callable[[str, int, int, int], NoReturn]=None,
                 new_subscription_handler: Callable[[str, int, int], NoReturn]=None,
                 available_image_handler: Callable[[Image], NoReturn]=None,
                 unavailable_image_handler: Callable[[Image], NoReturn]=None,
                 **kwargs) -> NoReturn: ...

    def add_subscription(self, channel: str, stream_id: int) -> Subscription: ...
    def add_publication(self, channel: str, stream_id: int) -> Publication: ...
    def add_exclusive_publication(self, channel: str, stream_id: int) -> ExclusivePublication: ...