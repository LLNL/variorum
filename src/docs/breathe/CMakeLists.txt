# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

include(ExternalProject)

message(STATUS "Adding external project: breathe@f61dc50")
ExternalProject_Add(breathe
  GIT_REPOSITORY https://github.com/michaeljones/breathe.git
  GIT_TAG f61dc50
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  SOURCE_DIR ${PROJECT_BINARY_DIR}/thirdparty_builtin/breathe
  PREFIX ${PROJECT_BINARY_DIR}/thirdparty_builtin/breathe-prefix
  CONFIGURE_COMMAND ""
  BUILD_COMMAND make SPHINXBUILD=${SPHINX_EXECUTABLE}
  INSTALL_COMMAND ""
  BUILD_IN_SOURCE 1
)
