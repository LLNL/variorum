# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

#######################################################################
# create config.mk for makefile-based build systems
#######################################################################

# extra sauce for needed for config.mk

# generated the config.mk
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/variorum_config.mk.in"
                "${CMAKE_CURRENT_BINARY_DIR}/variorum_config.mk")

# install the config.mk
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/variorum_config.mk"
        DESTINATION share/variorum)

###############################################################################
# export everything for a cmake build to be able to import with find_package
###############################################################################

if (NOT DEFINED VARIORUM_INSTALL_INCLUDE_DIR)
    set(VARIORUM_INSTALL_INCLUDE_DIR "include")
endif()

if (NOT DEFINED VARIORUM_INSTALL_CONFIG_DIR)
    set(VARIORUM_INSTALL_CONFIG_DIR "lib")
endif()

if (NOT DEFINED VARIORUM_INSTALL_LIB_DIR)
    set(VARIORUM_INSTALL_LIB_DIR "lib")
endif()

if (NOT DEFINED VARIORUM_INSTALL_BIN_DIR)
    set(VARIORUM_INSTALL_BIN_DIR "bin")
endif()

if (NOT DEFINED VARIORUM_INSTALL_SHARED_RESOURCES_DIR)
    set(VARIORUM_INSTALL_SHARED_RESOURCES_DIR "share/variorum")
endif()

if (NOT DEFINED VARIORUM_INSTALL_CMAKE_MODULE_DIR)
    set(VARIORUM_INSTALL_CMAKE_MODULE_DIR "${VARIORUM_INSTALL_CONFIG_DIR}/cmake")
endif()

include(CMakePackageConfigHelpers)

# write version heler
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/VariorumConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# write cmake package config file
configure_package_config_file(
  VariorumConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/VariorumConfig.cmake
  INSTALL_DESTINATION ${VARIORUM_INSTALL_CONFIG_DIR}
  PATH_VARS
    VARIORUM_INSTALL_INCLUDE_DIR
    VARIORUM_INSTALL_LIB_DIR
    VARIORUM_INSTALL_BIN_DIR
    VARIORUM_INSTALL_SHARED_RESOURCES_DIR
    VARIORUM_INSTALL_CMAKE_MODULE_DIR
)

# install everything needed
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/VariorumConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/VariorumConfigVersion.cmake
    variorum_setup_deps.cmake
    variorum_setup_targets.cmake
    DESTINATION ${VARIORUM_INSTALL_CMAKE_MODULE_DIR})
