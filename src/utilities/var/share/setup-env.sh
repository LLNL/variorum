# Copyright 2013-2022 Lawrence Livermore National Security, LLC and other
# Spack Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)


########################################################################
#
# This file is part of Spack and sets up the spack environment for bash,
# zsh, and dash (sh).  This includes environment modules and lmod support,
# and it also puts spack in your path. The script also checks that at least
# module support exists, and provides suggestions if it doesn't. Source
# it like this:
#
#    . /path/to/spack/share/spack/setup-env.sh
#
########################################################################


########################################################################
# Prepends directories to path, if they exist.
#      pathadd /path/to/dir            # add to PATH
# or   pathadd OTHERPATH /path/to/dir  # add to OTHERPATH
########################################################################
_var_pathadd() {
    # If no variable name is supplied, just append to PATH
    # otherwise append to that variable.
    _pa_varname=PATH
    _pa_new_path="$1"
    if [ -n "$2" ]; then
        _pa_varname="$1"
        _pa_new_path="$2"
    fi

    # Do the actual prepending here.
    eval "_pa_oldvalue=\${${_pa_varname}:-}"

    _pa_canonical=":$_pa_oldvalue:"
    if [ -d "$_pa_new_path" ] && \
       [ "${_pa_canonical#*:${_pa_new_path}:}" = "${_pa_canonical}" ];
    then
        if [ -n "$_pa_oldvalue" ]; then
            eval "export $_pa_varname=\"$_pa_new_path:$_pa_oldvalue\""
        else
            export $_pa_varname="$_pa_new_path"
        fi
    fi
}


# Determine which shell is being used
_var_determine_shell() {
    if [ -f "/proc/$$/exe" ]; then
        # If procfs is present this seems a more reliable
        # way to detect the current shell
        _sp_exe=$(readlink /proc/$$/exe)
        # Shell may contain number, like zsh5 instead of zsh
        basename ${_sp_exe} | tr -d '0123456789'
    elif [ -n "${BASH:-}" ]; then
        echo bash
    elif [ -n "${ZSH_NAME:-}" ]; then
        echo zsh
    else
        PS_FORMAT= ps -p $$ | tail -n 1 | awk '{print $4}' | sed 's/^-//' | xargs basename
    fi
}
_sp_shell=$(_var_determine_shell)


#
# Figure out where this file is.
#
if [ "$_sp_shell" = bash ]; then
    _sp_source_file="${BASH_SOURCE[0]:-}"
elif [ "$_sp_shell" = zsh ]; then
    _sp_source_file="${(%):-%N}"
else
    # Try to read the /proc filesystem (works on linux without lsof)
    # In dash, the sourced file is the last one opened (and it's kept open)
    _sp_source_file_fd="$(\ls /proc/$$/fd 2>/dev/null | sort -n | tail -1)"
    if ! _sp_source_file="$(readlink /proc/$$/fd/$_sp_source_file_fd)"; then
        # Last resort: try lsof. This works in dash on macos -- same reason.
        # macos has lsof installed by default; some linux containers don't.
        _sp_lsof_output="$(lsof -p $$ -Fn0 | tail -1)"
        _sp_source_file="${_sp_lsof_output#*n}"
    fi

    # If we can't find this script's path after all that, bail out with
    # plain old $0, which WILL NOT work if this is sourced indirectly.
    if [ ! -f "$_sp_source_file" ]; then
        _sp_source_file="$0"
    fi
fi

#
# Find root directory and add bin to path.
#
# We send cd output to /dev/null to avoid because a lot of users set up
# their shell so that cd prints things out to the tty.
if [ "$_sp_shell" = zsh ]; then
    _sp_share_dir="${_sp_source_file:A:h}"
    _sp_prefix="${_sp_share_dir:h:h}"
else
    _sp_share_dir="$(cd "$(dirname $_sp_source_file)" > /dev/null && pwd)"
    _sp_prefix="$(cd "$(dirname $(dirname $(dirname $(dirname $_sp_share_dir))))" > /dev/null && pwd)"
fi
if [ -x "$_sp_prefix/src/utilities/var/bin/var" ]; then
    export VAR_ROOT="${_sp_prefix}"
else
    # If the shell couldn't find the sourced script, fall back to
    # whatever the user set VAR_ROOT to.
    if [ -n "$VAR_ROOT" ]; then
        _sp_prefix="$VAR_ROOT"
        _sp_share_dir="$_sp_prefix/share/var"
    fi

    # If SPACK_ROOT didn't work, fail.  We should need this rarely, as
    # the tricks above for finding the sourced file are pretty robust.
    if [ ! -x "$_sp_prefix/src/utilities/var/bin/var" ]; then
        echo "==> Error: VAR_ROOT must point to var's prefix when using $_sp_shell"
        echo "Run this with the correct prefix before sourcing setup-env.sh:"
        echo "    export VAR_ROOT=</path/to/var>"
        return 1
    fi
fi
_var_pathadd PATH "${_sp_prefix%/}/src/utilities/var/bin"
_var_pathadd PYTHONPATH "${_sp_prefix%/}/src/utilities/var/cmd"


# Identify and lock the python interpreter
for cmd in "${VAR_PYTHON:-}" python3 python python2; do
    if command -v > /dev/null "$cmd"; then
        export VAR_PYTHON="$(command -v "$cmd")"
        break
    fi
done

# done: unset sentinel variable as we're no longer initializing
unset _sp_initializing
export _sp_initializing
