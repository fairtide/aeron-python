from datetime import datetime, timedelta
from time import sleep
from hamcrest import *
from pytest import fixture
from aeronpy import Context, Subscription


@fixture()
def context():
    return Context(resource_linger_timeout=timedelta(milliseconds=10))


@fixture()
def ipc_publication(context):
    return context.add_publication('aeron:ipc', 199)


@fixture()
def ipc_publication_2(context):
    return context.add_exclusive_publication('aeron:ipc', 199)


@fixture()
def mcast_publication(context):
    return context.add_exclusive_publication('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', 300)


def test_create__no_publisher():
    # always connected
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 87000)
    sleep(0.5)

    assert_that(subscription.is_connected, is_(True))
    assert_that(subscription.images, is_(empty()))


def test_create__with_publisher(ipc_publication):
    # always connected
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    sleep(0.5)

    assert_that(subscription.images, is_not(empty()))
    assert_that(subscription.images, has_length(1))
    assert_that(subscription.images[0].session_id, ipc_publication.session_id)


def test_poll__not_connected():
    # always connected
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 87000)

    received = list()
    result = subscription.poll(lambda data: received.append(data))
    assert_that(result, is_(0))


def test_poll__no_data(ipc_publication):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    sleep(0.5)

    assert_that(subscription.images, is_not(empty()))
    assert_that(subscription.images, has_length(1))
    assert_that(subscription.images[0].session_id, ipc_publication.session_id)

    received = list()
    result = subscription.poll(lambda data: received.append(data))
    assert_that(result, is_(0))


def test_poll__single_fragment(ipc_publication):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    sleep(0.5)

    ipc_publication.offer(b'abc')
    ipc_publication.offer(b'cde')
    ipc_publication.offer(b'efg')

    received = list()
    result = subscription.poll(lambda data: received.append(bytes(data)), fragment_limit=1)
    assert_that(result, is_(equal_to(1)))
    assert_that(received, has_length(1))
    assert_that(received[0], is_(equal_to(b'abc')))

    result = subscription.poll(lambda data: received.append(bytes(data)), fragment_limit=1)
    assert_that(result, is_(equal_to(1)))
    assert_that(received, has_length(2))
    assert_that(received[-1], is_(equal_to(b'cde')))


def test_poll__multiple_fragments(ipc_publication):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    sleep(0.5)

    ipc_publication.offer(b'abc')
    ipc_publication.offer(b'cde')
    ipc_publication.offer(b'efg')

    received = list()
    result = subscription.poll(lambda data: received.append(bytes(data)), fragment_limit=3)
    assert_that(result, is_(equal_to(3)))
    assert_that(received, has_length(3))
    assert_that(received[0], is_(equal_to(b'abc')))
    assert_that(received[1], is_(equal_to(b'cde')))
    assert_that(received[2], is_(equal_to(b'efg')))


def test_poll__multiple_sessions(ipc_publication, ipc_publication_2):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 199)
    sleep(0.5)

    assert_that(subscription.images, has_length(2))
    ipc_publication.offer(b'abc')
    ipc_publication_2.offer(b'cde')

    received = list()
    result = subscription.poll(lambda data, header: received.append((header.session_id, bytes(data))))
    assert_that(result, is_(equal_to(2)))
    assert_that(received, has_length(2))


def test_poll_eos__no_data(ipc_publication):
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 87000)
    result = subscription.poll_eos()
    assert_that(result, is_(0))


# def test_poll_eos__single_image(mcast_publication):
#     context = Context()
#     subscription = context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', 300)
#     sleep(0.5)
#
#     mcast_publication.offer(b'abc')
#     sleep(0.5)
#
#     result = subscription.poll(lambda _: None)
#     assert_that(result, is_(1))
#
#     mcast_publication.close()
#     del mcast_publication
#
#     sleep(30)
#
#     finished = list()
#
#     result = subscription.poll_eos(lambda image: finished.append(image.session_id))
#     assert_that(result, is_(1))
#
#
# def test_poll_eos__multiple_images(ipc_publication):
#     pass