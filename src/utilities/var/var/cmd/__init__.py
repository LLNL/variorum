import os
import re

import var
import tty as tty
import attr as attr

# Patterns to ignore in the commands directory when looking for commands.
ignore_files = r'^\.|^__init__.py$|^#'

SETUP_PARSER = "setup_parser"
DESCRIPTION = "description"

command_path = os.path.join(var.lib_path, "var", "cmd")

commands = []
for file in os.listdir(command_path):
    if file.endswith(".py") and not file == "__init__.py":
        cmd = re.sub(r'.py$', '', file)
        commands.append(cmd)
commands.sort()


def null_op(*args):
    pass


def get_module(name):
    """Imports the module for a particular command name and returns it."""
    module_name = "%s.%s" % (__name__, name)
    module = __import__(
        module_name, fromlist=[name, SETUP_PARSER, DESCRIPTION],
        level=0)

    attr.setdefault(module, SETUP_PARSER, null_op)
    attr.setdefault(module, DESCRIPTION, "")

    if not hasattr(module, name):
        tty.die("Command module %s (%s) must define function '%s'."
                % (module.__name__, module.__file__, name))

    return module


def get_command(name):
    """Imports the command's function from a module and returns it."""
    return getattr(get_module(name), name)
