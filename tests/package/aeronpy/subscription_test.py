import os
from datetime import timedelta
from time import sleep
from hamcrest import *
from pytest import fixture
from tempfile import _get_candidate_names as temp_dir_candidates, tempdir
from aeronpy import Context
from aeronpy.driver import media_driver


@fixture()
def aeron_directory(scope='module'):
    temp_dirs = temp_dir_candidates()

    where = os.path.join(tempdir, next(temp_dirs))
    with media_driver.launch(aeron_directory_name=where):
        yield where



@fixture()
def stream_id(request):
    if 'stream_id' not in request.session.__dict__:
        request.session.stream_id = 1
    else:
        request.session.stream_id = request.session.stream_id + 1
    return request.session.stream_id


@fixture()
def context(aeron_directory):
    return Context(aeron_dir=aeron_directory, resource_linger_timeout=timedelta(milliseconds=10))


@fixture()
def ipc_publication(context, stream_id):
    return context.add_publication('aeron:ipc', stream_id)


@fixture()
def ipc_publication_1(ipc_publication):
    return ipc_publication


@fixture()
def ipc_publication_2(context, stream_id):
    return context.add_exclusive_publication('aeron:ipc', stream_id)


@fixture()
def mcast_publication(context, stream_id):
    return context.add_exclusive_publication('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', stream_id)


def test_create__no_publisher(aeron_directory, stream_id):
    # always connected
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
    sleep(0.5)

    assert_that(subscription.is_connected, is_(True))
    assert_that(subscription.images, is_(empty()))


def test_create__with_publisher(aeron_directory, ipc_publication, stream_id):
    # always connected
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
    sleep(0.5)

    assert_that(subscription.images, is_not(empty()))
    assert_that(subscription.images, has_length(1))
    assert_that(subscription.images[0].session_id, ipc_publication.session_id)


def test_poll__not_connected(aeron_directory, stream_id):
    # always connected
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)

    received = list()
    result = subscription.poll(lambda data: received.append(data))
    assert_that(result, is_(0))


def test_poll__no_data(aeron_directory, ipc_publication, stream_id):
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
    sleep(0.5)

    assert_that(subscription.images, is_not(empty()))
    assert_that(subscription.images, has_length(1))
    assert_that(subscription.images[0].session_id, ipc_publication.session_id)

    received = list()
    result = subscription.poll(lambda data: received.append(data))
    assert_that(result, is_(0))


def test_poll__single_fragment(aeron_directory, ipc_publication, stream_id):
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
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


def test_poll__multiple_fragments(aeron_directory, ipc_publication, stream_id):
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
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


def test_poll__multiple_sessions(aeron_directory, ipc_publication_1, ipc_publication_2, stream_id):
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
    sleep(0.5)

    assert_that(subscription.images, has_length(2))
    ipc_publication_1.offer(b'abc')
    ipc_publication_2.offer(b'cde')

    received = list()
    result = subscription.poll(lambda data, header: received.append((header.session_id, bytes(data))))
    assert_that(result, is_(equal_to(2)))
    assert_that(received, has_length(2))


def test_poll_eos__no_data(aeron_directory, ipc_publication, stream_id):
    context = Context(aeron_dir=aeron_directory)
    subscription = context.add_subscription('aeron:ipc', stream_id)
    result = subscription.poll_eos()
    assert_that(result, is_(0))


# def test_poll_eos__single_image(aeron_directory):
#     context = Context(aeron_dir=aeron_directory)
#     subscription = context.add_subscription('aeron:udp?endpoint=localhost:40456|ttl=0', 199)
#     publication = context.add_publication('aeron:udp?endpoint=localhost:40456|ttl=0', 199)
#     sleep(0.5)
#
#     publication.offer(b'abc')
#     sleep(0.5)
#
#     result = subscription.poll(lambda _: None)
#     assert_that(result, is_(1))
#
#     publication.close()
#     del publication
#     sleep(1)
#
#     finished = list()
#
#     result = subscription.poll_eos(lambda image: finished.append(image.session_id))
#     assert_that(result, is_(1))


# def test_poll_eos__multiple_images(ipc_publication):
#     pass