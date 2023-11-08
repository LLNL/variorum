#!/bin/bash

# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# check format for all *.cpp, *.h, and *.c files in src/ directory
#   exclude files in thirdparty_builtin/ (gtest source files)
#   exclude files in fortran/ (EP source files)
FILES=$(find src -type f  \( -name "*.[ch]" -o -name *.cpp \) \
        | grep -v thirdparty_builtin \
        | grep -v fortran)

astyle --errors-to-stdout \
       --preserve-date \
       --style=allman \
       --indent=spaces=4 \
       --convert-tabs \
       --attach-extern-c \
       --indent-col1-comments \
       --min-conditional-indent=0 \
       --max-instatement-indent=40 \
       --pad-oper \
       --pad-header \
       --unpad-paren \
       --align-pointer=name \
       --align-reference=name \
       --break-closing-braces \
       --keep-one-line-blocks \
       --keep-one-line-statements \
       --max-code-length=80 \
       --break-after-logical \
       --indent-switches \
       --formatted \
       ${FILES}

#
