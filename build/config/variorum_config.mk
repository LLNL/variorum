# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

VARIORUM_DIR = /usr/local

VARIORUM_HWLOC_INCLUDE_DIR = /usr/include
VARIORUM_HWLOC_LIB_DIR = /usr/lib/x86_64-linux-gnu/libhwloc.so

VARIORUM_JANSSON_INCLUDE_DIR = /usr/include
VARIORUM_JANSSON_LIB_DIR = /usr/lib/x86_64-linux-gnu/libjansson.so

# setup rpath helper
VARIORUM_LINK_RPATH = -Wl,-rpath,$(VARIORUM_DIR)/lib

VARIORUM_INCLUDE_FLAGS = -I $(VARIORUM_DIR)/include \
						 -I $(VARIORUM_HWLOC_INCLUDE_DIR) \
						 -I $(VARIORUM_JANSSON_INCLUDE_DIR)

VARIORUM_LIB_FLAGS = -L $(VARIORUM_DIR)/lib -lvariorum \
					 -L $(VARIORUM_HWLOC_LIB_DIR) \
					 -L $(VARIORUM_JANSSON_LIB_DIR)
