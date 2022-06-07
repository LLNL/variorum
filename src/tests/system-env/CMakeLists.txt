# Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

if(VARIORUM_WITH_INTEL)
  add_subdirectory("intel")
endif()

if(VARIORUM_WITH_ARM)
  add_subdirectory("ARM")
endif()
