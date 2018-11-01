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


# TODO: add replay tests