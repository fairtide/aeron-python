import os
from subprocess import Popen


class MediaDriver(object):
    """
    Helper type providing interface for finding, starting, stopping
    media driver for duration of tests.
    """
    def __init__(self, aeron_dir: str=None):
        for root, dirs, files in os.walk(aeron_dir):
            pass

    def start(self):
        Popen(['java', '-cp', ])

    def stop(self):
        pass

    def __enter__(self):
        self.start()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()
        return None