from datetime import datetime
from time import sleep
from hamcrest import *
from aeronpy import Context


def test_create__default():
    context = Context()
    assert_that(context, is_not(None))


def test_create__wrong_parameter_type():
    assert_that(calling(Context).with_args(aeron_dir=datetime(2018, 1, 1)), raises(RuntimeError))
    assert_that(calling(Context).with_args(media_driver_timeout='abc'), raises(RuntimeError))
    assert_that(calling(Context).with_args(resource_linger_timeout='abc'), raises(RuntimeError))
    assert_that(calling(Context).with_args(user_conductor_agent_invoker='abc'), raises(RuntimeError))
    assert_that(calling(Context).with_args(error_handler='abc'), raises(TypeError))
    assert_that(calling(Context).with_args(new_publication_handler='abc'), raises(TypeError))
    assert_that(calling(Context).with_args(new_exclusive_publication_handler='abc'), raises(TypeError))
    assert_that(calling(Context).with_args(new_subscription_handler='abc'), raises(TypeError))
    assert_that(calling(Context).with_args(available_image_handler='abc'), raises(TypeError))
    assert_that(calling(Context).with_args(unavailable_image_handler='abc'), raises(TypeError))


def test_add_subscription():
    context = Context()
    subscription = context.add_subscription('aeron:ipc', 1000)
    assert_that(subscription, is_not(None))
    assert_that(subscription.channel, is_('aeron:ipc'))
    assert_that(subscription.stream_id, is_(1000))


def test_add_subscription__with_handlers():
    class Handler:
        def __init__(self):
            self.subscriptions = list()

        def on_new_subscription(self, *args):
            self.subscriptions.append(args)

    handler = Handler()
    context = Context(new_subscription_handler=handler.on_new_subscription)
    subscription = context.add_subscription('aeron:ipc', 546)

    assert_that(handler.subscriptions, is_not(empty()))
    assert_that(handler.subscriptions, has_length(equal_to(1)))
    assert_that(handler.subscriptions[0][0], is_('aeron:ipc'))
    assert_that(handler.subscriptions[0][1], is_(546))


def test_add_publication():
    context = Context()
    publication = context.add_publication('aeron:ipc', 2000)
    assert_that(publication, is_not(None))
    assert_that(publication.channel, is_('aeron:ipc'))
    assert_that(publication.stream_id, is_(2000))


def test_add_publication__wrong_channel():
    context = Context()
    assert_that(calling(context.add_publication).with_args('wrong channel', 1), raises(RuntimeError))


def test_add_publication__with_handler():
    class Handler(object):
        def __init__(self):
            self.publications = list()

        def on_new_publication(self, *args):
            self.publications.append(args)

    handler = Handler()
    context = Context(new_publication_handler=handler.on_new_publication)
    publication = context.add_publication('aeron:ipc', 3000)

    assert_that(handler.publications, is_not(empty()))
    assert_that(handler.publications, has_length(equal_to(1)))
    assert_that(handler.publications[0][0], is_(equal_to('aeron:ipc')))
    assert_that(handler.publications[0][1], is_(equal_to(3000)))


def test_add_exclusive_publication():
    context = Context()
    publication = context.add_exclusive_publication('aeron:ipc', 4000)
    assert_that(publication, is_not(None))
    assert_that(publication.channel, is_('aeron:ipc'))
    assert_that(publication.stream_id, is_(4000))


def test_add_exclusive_publication__with_handler():
    class Handler(object):
        def __init__(self):
            self.publications = list()

        def on_new_exclusive_publication(self, *args):
            self.publications.append(args)

    handler = Handler()
    context = Context(new_exclusive_publication_handler=handler.on_new_exclusive_publication)
    publication = context.add_exclusive_publication('aeron:ipc', 5000)

    assert_that(handler.publications, is_not(empty()))
    assert_that(handler.publications, has_length(equal_to(5000)))
    assert_that(handler.publications[0][0], is_(equal_to('aeron:ipc')))
    assert_that(handler.publications[0][1], is_(equal_to(5000)))


def test_image_available_unavailable_callbacks():
    class Handler(object):
        def __init__(self):
            self.available_images = list()
            self.unavailable_images = list()

        def on_image_available(self, *args):
            self.available_images.append(args)

        def on_image_unavailable(self, *args):
            self.unavailable_images.append(args)

    handler = Handler()
    context = Context(available_image_handler=handler.on_image_available,
                      unavailable_image_handler=handler.on_image_unavailable)
    subscription = context.add_subscription('aeron:ipc', 6000)

    publication_context = Context()
    publication = publication_context.add_publication('aeron:ipc', 6000)
    sleep(1)

    publication.close()
    del publication
    sleep(1)

    assert_that(handler.available_images, is_not(empty()))
    assert_that(handler.unavailable_images, is_not(empty()))


