from time import sleep
from hamcrest import *
from pytest import fixture
from tempfile import mkdtemp
from aeronpy import Context, NOT_CONNECTED, PUBLICATION_CLOSED
from aeronpy.driver import media_driver


@fixture()
def aeron_directory():
    where = mkdtemp(prefix='aeron_dir')
    with media_driver.launch(aeron_directory_name=where):
        yield where


@fixture()
def context(aeron_directory):
    return Context(aeron_dir=aeron_directory)


@fixture()
def ipc_subscriber(context):
    return context.add_subscription('aeron:ipc', 10)


@fixture()
def mcast_subscriber(context):
    return context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', 20)


def test_offer__ipc__no_subscribers(aeron_directory):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', 10)
    assert_that(publication.is_connected, is_(False))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(NOT_CONNECTED)))


def test_offer__ipc__message_too_large(aeron_directory, ipc_subscriber):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', 10)

    blob = bytearray(50 * 1024 * 1024)
    assert_that(calling(publication.offer).with_args(blob), raises(RuntimeError))


def test_offer__ipc__closed(aeron_directory, ipc_subscriber):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', 10)
    publication.close()
    assert_that(publication.is_closed, is_(True))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(PUBLICATION_CLOSED)))


def test_offer__ipc(aeron_directory, ipc_subscriber):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:ipc', 10)
    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multicast(aeron_directory, mcast_subscriber):
    context = Context(aeron_dir=aeron_directory)
    publication = context.add_exclusive_publication('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', 20)
    sleep(0.5)

    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multiple_publishers__same_stream(aeron_directory, ipc_subscriber):
    context = Context(aeron_dir=aeron_directory)
    publication_1 = context.add_exclusive_publication('aeron:ipc', 10)
    publication_2 = context.add_exclusive_publication('aeron:ipc', 10)
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

