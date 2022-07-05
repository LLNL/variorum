#!/bin/bash

# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

# re-format all *.rst files in src/docs directory
FILES=$(find src -type f  \( -name *.rst \))

rstfmt -w 80 ${FILES}

#
