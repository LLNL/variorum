#!/bin/bash

# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

./scripts/check-rst-format.sh

RES=$(git ls-files -m)

echo -e "$RES"

if [ -z "$RES" ]; then
    exit 0
else
    echo -e "Formatting issue found in RST files. Please run ./scripts/check-rst-format.sh."
    exit 1
fi

#
