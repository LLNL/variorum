#!/bin/bash

# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# check format for all *.cpp, *.h, and *.c files in src/ directory
#   exclude files in thirdparty_builtin/ (gtest source files)
#   exclude files in fortran/ (EP source files)
function check_format_files_cpp_c_h {
    if ! command -v astyle &> /dev/null; then
        echo -e "'astyle' not found: see https://astyle.sourceforge.net/ for installation info"
        exit
    fi
    echo -e "Checking format of cpp/c/h files using astyle"

    ./scripts/check-cpp-c-format.sh
}

function check_format_files_rst {
    if ! command -v rstfmt &> /dev/null; then
        echo -e "'rstfmt' not found: this package can be installed using pip"
        exit
    fi
    echo -e "Checking format of rst documentation using rstfmt"

    ./scripts/check-rst-format.sh
}

function check_format_files_python {
    if ! command -v flake8 &> /dev/null; then
        echo -e "'flake8' not found: this package can be installed using pip"
        exit
    fi
    if ! command -v black &> /dev/null; then
        echo -e "'black' not found: this package can be installed using pip"
        exit
    fi
    echo -e "Checking format of python files using flake and black"

    flake8
    black --diff --check --exclude "/(src/thirdparty_builtin/googletest|build|src/docs)/" .
}

check_format_files_cpp_c_h
check_format_files_rst
check_format_files_python

#
