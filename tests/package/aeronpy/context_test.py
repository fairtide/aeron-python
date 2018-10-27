from hamcrest import *
from aeronpy import Context


def test_create__default():
    context = Context()
    assert_that(context, is_not(None))


def test_add_subscription():
    context = Context()
    subscription = context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', 1)
    assert_that(subscription, is_not(None))
    assert_that(subscription.channel, is_('aeron:udp?endpoint=224.0.1.1:40456'))
    assert_that(subscription.stream_id, is_(1))


def test_add_subscription__with_handlers():
    class Handler:
        def __init__(self):
            self.publications = list()

        def on_new_publication(self, channel, stream, session ,correlation_id):
            self.publications.append((channel, stream, session, correlation_id))

    handler = Handler()
    context = Context(new_publication_handler=handler.on_new_publication)
    subscription = context.add_subscription('aeron:udp?endpoint=224.0.1.1:40456', 1)
    assert_that(subscription, is_not(None))
    assert_that(subscription.channel, is_('aeron:udp?endpoint=224.0.1.1:40456'))
    assert_that(subscription.stream_id, is_(1))
    assert_that(handler.publications, is_not(empty()))


def test_add_publication():
    with Context() as context:
        publication = context.add_publication('aeron:udp?endpoint=224.0.1.1:40456', 1)
        assert_that(publication, is_not(None))
        assert_that(publication.channel, is_('aeron:udp?endpoint=224.0.1.1:40456'))
        assert_that(publication.stream_id, is_(1))
        del publication