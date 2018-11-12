import os
from time import sleep
from hamcrest import *
from pytest import fixture
from tempfile import _get_candidate_names as temp_dir_candidates, tempdir
from aeronpy import Context, NOT_CONNECTED, PUBLICATION_CLOSED
from aeronpy.driver import media_driver


@fixture(scope='module')
def aeron_directory():
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
    return Context(aeron_dir=aeron_directory)


@fixture()
def ipc_subscriber(context, stream_id):
    return context.add_subscription('aeron:ipc', stream_id)


@fixture()
def mcast_subscriber(context, stream_id):
    return context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', stream_id)


def test_offer__ipc__no_subscribers(aeron_directory, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', stream_id)
    assert_that(publication.is_connected, is_(False))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(NOT_CONNECTED)))


def test_offer__ipc__message_too_large(aeron_directory, ipc_subscriber, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', stream_id)

    blob = bytearray(50 * 1024 * 1024)
    assert_that(calling(publication.offer).with_args(blob), raises(RuntimeError))


def test_offer__ipc__closed(aeron_directory, ipc_subscriber, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', stream_id)
    publication.close()
    assert_that(publication.is_closed, is_(True))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(PUBLICATION_CLOSED)))


def test_offer__ipc(aeron_directory, ipc_subscriber, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', stream_id)
    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multicast(aeron_directory, mcast_subscriber, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', stream_id)
    sleep(0.5)

    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multiple_publishers__same_stream(aeron_directory, ipc_subscriber, stream_id):
    context = Context(aeron_dir=aeron_directory)
    publication_1 = context.add_exclusive_publication('aeron:ipc', stream_id)
    publication_2 = context.add_exclusive_publication('aeron:ipc', stream_id)
    assert_that(publication_1.session_id, is_not(equal_to(publication_2.session_id)))

    result = publication_1.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))

    result = publication_2.offer(b'def')
    assert_that(result, is_(greater_than_or_equal_to(0)))

    sleep(0.5)

    messages = list()
    ipc_subscriber.poll(lambda data: messages.append(data))

    assert_that(messages, has_length(equal_to(2)))
    assert_that(bytes(messages[0]), is_(equal_to(b'abc')))
    assert_that(bytes(messages[1]), is_(equal_to(b'def')))

