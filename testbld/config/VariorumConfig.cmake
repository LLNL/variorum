# Copyright 2019-2023 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was VariorumConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

if(NOT VARIORUM_FOUND)
    set(VARIORUM_VERSION "0.5.0")
    set(VARIORUM_INSTALL_PREFIX "/usr/local")
    set(VARIORUM_HWLOC_DIR "/usr/include/hwloc")
    set(VARIORUM_HWLOC_LIB "/usr/lib/x86_64-linux-gnu/libhwloc.so")
    set(VARIORUM_HWLOC_INCLUDE_DIR "/usr/include")
    set(VARIORUM_JANSSON_DIR "/usr/include")
    set(VARIORUM_JANSSON_LIB "/usr/lib/x86_64-linux-gnu/libjansson.so")
    set(VARIORUM_JANSSON_INCLUDE_DIR "/usr/include")

    # pull in vars with details about configured paths
    get_filename_component(VARIORUM_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

    # setup dependent pkgs
    include("${VARIORUM_CMAKE_CONFIG_DIR}/variorum_setup_deps.cmake")

    # include the main exported targets
    include("${VARIORUM_CMAKE_CONFIG_DIR}/variorum.cmake")

    # finish setup
    include("${VARIORUM_CMAKE_CONFIG_DIR}/variorum_setup_targets.cmake")

    set(VARIORUM_FOUND TRUE)
endif()
