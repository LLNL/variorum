import os
import re

class Version(object):
    """Class to represent versions"""
    def __init__(self, version_string):
        self.version_string = version_string
        self.version = canonical(version_string)

    def __cmp__(self, other):
        return cmp(self.version, other.version)

    @property
    def major(self):
        return self.component(0)

    @property
    def minor(self):
        return self.component(1)

    @property
    def patch(self):
        return self.component(2)

    def component(self, i):
        """Returns the ith version component"""
        if len(self.version) > i:
            return self.version[i]
        else:
            return None

    def __repr__(self):
        return self.version_string

    def __str__(self):
        return self.version_string


def canonical(v):
    """Get a "canonical" version of a version string, as a tuple."""
    def intify(part):
        try:
            return int(part)
        except:
            return part
    return tuple(intify(v) for v in re.split(r'[_.-]+', v))
