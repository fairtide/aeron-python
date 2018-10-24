from hamcrest import *
from aeronpy import Context


def test_create():
    with Context() as context:
        pass


def test_add_subscription():
    with Context() as context:
        subscription = context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', 1)
        assert_that(subscription, is_not(None))
        assert_that(subscription.channel, is_('aeron:udp?endpoint=224.0.1.1:40456'))
        assert_that(subscription.stream_id, is_(1))
        del subscription


def test_add_publication():
    with Context() as context:
        publication = context.add_publication('aeron:udp?endpoint=224.0.1.1:40456', 1)
        assert_that(publication, is_not(None))
        assert_that(publication.channel, is_('aeron:udp?endpoint=224.0.1.1:40456'))
        assert_that(publication.stream_id, is_(1))
        del publication