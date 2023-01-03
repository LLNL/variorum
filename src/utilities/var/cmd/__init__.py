import os
import re

def cmd_name(python_name):
    """Convert module name (with ``_``) to command name (with ``-``)."""
    return python_name.replace("_", "-")

#: global, cached list of all commands -- access through all_commands()
_all_commands = None


def all_commands():
    """Get a sorted list of all spack commands.
    This will list the lib/spack/spack/cmd directory and find the
    commands there to construct the list.  It does not actually import
    the python files -- just gets the names.
    """
    global _all_commands
    if _all_commands is None:
        _all_commands = []
        for path in ["cmd"]:
            for file in os.listdir(path):
                if file.endswith(".py"):
                    cmd = re.sub(r'.py$', '', file)
                    _all_commands.append(cmd_name(cmd))
                    print("appending", cmd)

        _all_commands.sort()

    return _all_commands
