from time import sleep
from hamcrest import *
from pytest import fixture
from aeronpy import Context, NOT_CONNECTED, PUBLICATION_CLOSED


@fixture()
def context():
    return Context()


@fixture()
def ipc_subscriber(context):
    return context.add_subscription('aeron:ipc', 50)


@fixture()
def mcast_subscriber(context):
    return context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', 20)


def test_str_():
    context = Context()
    publication = context.add_publication('aeron:ipc', 9845)

    assert_that(str(publication), contains_string('aeron:ipc'))
    assert_that(str(publication), contains_string('9845'))


def test_offer__ipc__no_subscribers():
    context = Context()
    publication = context.add_publication('aeron:ipc', 50)
    assert_that(publication.is_connected, is_(False))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(NOT_CONNECTED)))


def test_offer__ipc__message_too_large(ipc_subscriber):
    context = Context()
    publication = context.add_publication('aeron:ipc', 50)

    blob = bytearray(50 * 1024 * 1024)
    assert_that(calling(publication.offer).with_args(blob), raises(RuntimeError))


def test_offer__ipc__closed(ipc_subscriber):
    context = Context()
    publication = context.add_publication('aeron:ipc', 50)
    publication.close()
    assert_that(publication.is_closed, is_(True))

    result = publication.offer(b'abc')
    assert_that(result, is_(equal_to(PUBLICATION_CLOSED)))


def test_offer__ipc(ipc_subscriber):
    context = Context()
    publication = context.add_publication('aeron:ipc', 50)
    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multicast(mcast_subscriber):
    context = Context()
    publication = context.add_publication('aeron:udp?endpoint=224.0.1.1:40456|ttl=0', 20)
    sleep(0.5)

    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than_or_equal_to(0)))


def test_offer__multiple_publishers__same_stream():
    subscriber_context = Context()
    ipc_subscriber = subscriber_context.add_subscription('aeron:ipc', 75)

    context = Context()
    publication_1 = context.add_publication('aeron:ipc', 75)
    publication_2 = context.add_publication('aeron:ipc', 75)
    assert_that(publication_1.session_id, is_(equal_to(publication_2.session_id)))

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

