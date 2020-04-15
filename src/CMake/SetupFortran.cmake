# Copyright 2019-2020 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

###############################
# Guards for Fortran support. #
###############################
if(ENABLE_FORTRAN)
    if(CMAKE_Fortran_COMPILER)
        message(STATUS "Fortran Compiler: ${CMAKE_Fortran_COMPILER}")
        set(CMAKE_Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/fortran)
    elseif(CMAKE_GENERATOR STREQUAL Xcode)
        message(STATUS "Disabling Fortran support: ENABLE_FORTRAN is true, "
                       "but the Xcode CMake Generator does not support Fortran.")
        set(ENABLE_FORTRAN OFF)
    else()
        message(FATAL_ERROR "ENABLE_FORTRAN is true, but a Fortran compiler wasn't found.")
    endif()
    set(FORTRAN_FOUND 1)
endif()

