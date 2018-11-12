import os
from hamcrest import *
from pytest import fixture
from tempfile import _get_candidate_names as temp_dir_candidates, tempdir
from time import sleep
from aeronpy import Archive
from aeronpy.driver import archiving_media_driver


@fixture()
def aeron_directory():
    temp_dirs = temp_dir_candidates()

    where = os.path.join(tempdir, next(temp_dirs))
    where_archive = os.path.join(tempdir, next(temp_dirs))
    with archiving_media_driver.launch(aeron_directory_name=where, archive_directory_name=where_archive):
        yield where


@fixture()
def config_file():
    here, _ = os.path.split(__file__)
    return os.path.join(here, 'archive.properties')


def test__archive_create(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory)
    assert_that(archive, is_not(None))


def test__archive_create__with_config(aeron_directory, config_file):
    archive = Archive(config_file=config_file, aeron_dir=aeron_directory)
    assert_that(archive, is_not(None))


def test__archive_add_recorded_publication(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    recording = archive.find_last('aeron:ipc', 5000)
    assert_that(recording, is_(None))

    publication = archive.add_recorded_publication('aeron:ipc', 5000)

    sleep(0.5)

    recording = archive.find_last('aeron:ipc', 5000)
    assert_that(recording, is_not(None))
    assert_that(recording.id, is_(equal_to(0)))

    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than(0)))

    sleep(0.5)

    assert_that(recording.position, is_(equal_to(result)))


def test__archive_add_recorded_exclusive_publication(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    recording = archive.find_last('aeron:ipc', 5000)
    assert_that(recording, is_(None))

    publication = archive.add_recorded_exclusive_publication('aeron:ipc', 5000)

    sleep(0.5)

    recording = archive.find_last('aeron:ipc', 5000)
    assert_that(recording, is_not(None))
    assert_that(recording.id, is_(equal_to(0)))

    result = publication.offer(b'abc')
    assert_that(result, is_(greater_than(0)))

    sleep(0.5)

    assert_that(recording.position, is_(equal_to(result)))


def test__recording_find(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    publication = archive.add_recorded_publication('aeron:ipc', 5000)
    sleep(0.5)

    recording = archive.find(0)
    assert_that(recording, is_not(None))
    assert_that(recording.position, is_(equal_to(0)))


def test__recording_replay(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    publication = archive.add_recorded_publication('aeron:ipc', 5000)
    offer_result = publication.offer(b'abc')
    assert_that(offer_result, is_(greater_than(0)))

    offer_result = publication.offer(b'def')
    assert_that(offer_result, is_(greater_than(0)))

    sleep(0.5)

    recording = archive.find_last('aeron:ipc', 5000)
    subscription = recording.replay('aeron:ipc', 6000)
    assert_that(archive.find_last('aeron:ipc', 6000), is_(None))

    replayed = list()
    subscription.poll(lambda data: replayed.append(bytes(data)))
    assert_that(replayed, has_length(2))
    assert_that(replayed, has_items(equal_to(b'abc'), equal_to(b'def')))


def test__recording_replay__from_position(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    publication = archive.add_recorded_publication('aeron:ipc', 5000)
    offer_result = publication.offer(b'abc')
    assert_that(offer_result, is_(greater_than(0)))

    offer_result = publication.offer(b'def')
    assert_that(offer_result, is_(greater_than(0)))

    sleep(0.5)

    recording = archive.find_last('aeron:ipc', 5000)
    subscription = recording.replay('aeron:ipc', 6000, 64)
    assert_that(archive.find_last('aeron:ipc', 6000), is_(None))

    replayed = list()
    subscription.poll(lambda data: replayed.append(bytes(data)))
    assert_that(replayed, has_length(1))
    assert_that(replayed, has_items(equal_to(b'def')))


def test__recording_replay__from_position__not_aligned(aeron_directory):
    archive = Archive(aeron_dir=aeron_directory, aeron_archive_dir=aeron_directory)
    publication = archive.add_recorded_publication('aeron:ipc', 5000)
    offer_result = publication.offer(b'abc')
    assert_that(offer_result, is_(greater_than(0)))

    offer_result = publication.offer(b'def')
    assert_that(offer_result, is_(greater_than(0)))

    sleep(0.5)

    recording = archive.find_last('aeron:ipc', 5000)
    assert_that(calling(recording.replay).with_args('aeron:ipc', 6000, 50), raises(RuntimeError))
