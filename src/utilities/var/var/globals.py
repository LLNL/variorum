import os
import re
import multiprocessing
from var.version import Version

import var.tty
from var.utils import *

# This lives in $prefix/var/var/__file__
prefix = ancestor(__file__, 3)

# The var script itself
var_file = new_path(prefix, "bin", "var")

# var directory hierarchy
lib_path      = new_path(prefix, "var")
module_path   = new_path(lib_path, "var")

# Version information
var_version = Version("0.1")

verbose = False
debug = False

VAR_LIB = 'VAR_LIB'
VAR_ENV_PATH = 'VAR_ENV_PATH'
VAR_DEPENDENCIES = 'VAR_DEPENDENCIES'
VAR_PREFIX = 'VAR_PREFIX'
VAR_BUILD_ROOT = 'VAR_BUILD_ROOT'
