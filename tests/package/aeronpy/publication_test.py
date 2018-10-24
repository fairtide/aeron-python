from hamcrest import *
from time import sleep
from aeronpy import Context, Publication


def test_offer__string():
    context = Context()
    publication = context.add_publication('aeron:udp?endpoint=localhost:40123', 10)

    while not publication:
        pass
    assert_that(bool(publication), is_(True))

    publication.offer('abc')
