from datetime import datetime, timedelta
from hamcrest import *
from pytest import fixture
from aeronpy import Context, Subscription


@fixture()
def context():
    return Context()


@fixture()
def ipc_publication(context):
    return context.add_publication('aeron:ipc', 199)


def test_poll__not_connected():
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    assert_that(subscription.is_connected, is_(False))


def test_poll__no_data(ipc_publication):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)

# def test_poll():
#     context = Context()
#     subscription = context.add_subscription('aeron:udp?endpoint=localhost:40123', 10)
#     assert_that(bool(subscription), is_(True))
#
#     class Handler(object):
#         def __init__(self):
#             self.received = list()
#
#         def on_message(self, data):
#             self.received.append(bytes(data))
#
#     handler = Handler()
#     start = datetime.now()
#     while True:
#         result = subscription.poll(handler.on_message)
#         if result > 0:
#             break
#
#         if datetime.now() - start > timedelta(seconds=5):
#             fail('timeout reached')
#
#     assert_that(handler.received, is_not(empty()))
