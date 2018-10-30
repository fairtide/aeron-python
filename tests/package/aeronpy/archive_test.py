import os
from hamcrest import *
from pytest import fixture
from aeronpy import Archive


@fixture()
def config_file():
    here, _ = os.path.split(__file__)
    return os.path.join(here, 'archive.properties')


def test_create():
    archive = Archive('aeron:ipc', 233)
    assert_that(archive, is_not(None))


def test_create__with_config(config_file):
    archive = Archive('aeron:ipc', 233, config_file=config_file)
    assert_that(archive, is_not(None))


# def test_create__valid_channel_stream():
#     archive = Archive('aeron:udp?endpoint=localhost:40123', 10)
#     subscription = archive.replay('aeron:udp?endpoint=localhost:40128', 30)
#
#     class Handler(object):
#         def __init__(self):
#             self.received = list()
#
#         def on_data(self, data):
#             self.received.append(bytes(data))
#
#     handler = Handler()
#     while subscription:
#         if subscription.poll(handler.on_data) == 0:
#             if subscription.poll_eos() > 0:
#                 break
#     pass
#
#
# def test_create__valid_channel_stream__remote():
#     archive = Archive('aeron:udp?endpoint=224.0.1.1:40124', 42, config_file='/home/lukasz/projects/aeron-python/tests/package/aeronpy/archive.properties')
#     subscription = archive.replay('aeron:udp?endpoint=224.0.2.1:40128', 30)
#
#     class Handler(object):
#         def __init__(self):
#             self.received = list()
#
#         def on_data(self, data):
#             self.received.append(bytes(data))
#
#     handler = Handler()
#     while subscription:
#         if subscription.poll(handler.on_data) == 0:
#             if subscription.poll_eos() > 0:
#                 break
#     pass