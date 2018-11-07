import os
from hamcrest import *
from pytest import fixture
from tempfile import mkdtemp
from aeronpy import Archive
from aeronpy.driver import archiving_media_driver


@fixture()
def aeron_directories():
    where = mkdtemp(prefix='archive_test')
    where_archive = mkdtemp(prefix='archive_test', suffix='archive')
    with archiving_media_driver.launch(aeron_directory_name=where, archive_directory_name=where_archive):
        yield where, where_archive


@fixture()
def config_file():
    here, _ = os.path.split(__file__)
    return os.path.join(here, 'archive.properties')


def test_create(aeron_directories):
    aeron_directory, archive_directory = aeron_directories

    archive = Archive('aeron:ipc', 233)
    assert_that(archive, is_not(None))


def test_create__with_config(config_file):
    archive = Archive('aeron:ipc', 233, config_file=config_file)
    assert_that(archive, is_not(None))


# TODO: add replay tests