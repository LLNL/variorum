#!/bin/bash

# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

TMP=$(./scripts/check-rst-format.sh)

RES=$(git ls-files -m)

echo -e "$RES"

if [ -z "$RES" ]; then
    exit 0
else
    exit 1
fi

#
