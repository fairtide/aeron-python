from datetime import datetime, timedelta
from hamcrest import *
from pytest import fail

from aeronpy import Context, Subscription


def test_poll():
    context = Context()
    subscription = context.add_subscription('aeron:udp?endpoint=localhost:40123', 10)
    assert_that(bool(subscription), is_(True))

    class Handler(object):
        def __init__(self):
            self.received = list()

        def on_message(self, buffer, offset, length, header):
            data = bytes(buffer)[offset:offset + length]
            self.received.append(data)

    handler = Handler()
    start = datetime.now()
    while True:
        result = subscription.poll(handler.on_message)
        if result > 0:
            break

        if datetime.now() - start > timedelta(seconds=5):
            fail('timeout reached')

    assert_that(handler.received, is_not(empty()))
