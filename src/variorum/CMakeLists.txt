# Copyright 2019-2021 Lawrence Livermore National Security, LLC and other
# Variorum Project Developers. See the top-level LICENSE file for details.
#
# SPDX-License-Identifier: MIT

set(VARIORUM_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} CACHE INTERNAL "")

if(FORTRAN_FOUND)
    set(VARIORUM_FORTRAN_COMPILER ${CMAKE_Fortran_COMPILER})
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/variorum_config.h.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/variorum_config.h")

set(variorum_headers
  config_architecture.h
  variorum.h
  variorum_timers.h
  variorum_error.h
)

set(variorum_sources
  config_architecture.c
  variorum.c
  variorum_timers.c
  variorum_error.c
)

set(variorum_deps ""
    CACHE INTERNAL ""
)

set(variorum_includes
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${HWLOC_INCLUDE_DIRS}
    ${ESMI_INCLUDE_DIRS}
)

if(VARIORUM_WITH_INTEL)
    add_subdirectory(Intel)
    list(APPEND variorum_headers Intel/config_intel.h ${variorum_intel_headers})
    list(APPEND variorum_sources Intel/config_intel.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_intel>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/Intel)
endif()

if(VARIORUM_WITH_IBM)
    add_subdirectory(IBM)
    list(APPEND variorum_headers IBM/config_ibm.h ${variorum_ibm_headers})
    list(APPEND variorum_sources IBM/config_ibm.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_ibm>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/IBM)
endif()

if(VARIORUM_WITH_NVIDIA)
    add_subdirectory(Nvidia)
    list(APPEND variorum_headers Nvidia/config_nvidia.h ${variorum_nvidia_headers} ${NVML_INCLUDE_DIRS}/nvml.h)
    list(APPEND variorum_sources Nvidia/config_nvidia.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_nvidia>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/Nvidia ${NVML_INCLUDE_DIRS})
endif()

if(VARIORUM_WITH_ARM)
    add_subdirectory(ARM)
    list(APPEND variorum_headers ARM/config_arm.h ${variorum_arm_headers})
    list(APPEND variorum_sources ARM/config_arm.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_arm>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/ARM)
endif()

if(VARIORUM_WITH_AMD)
    add_subdirectory(AMD)
    list(APPEND variorum_headers AMD/config_amd.h ${variorum_amd_headers})
    list(APPEND variorum_sources AMD/config_amd.c)
    list(APPEND variorum_deps $<TARGET_OBJECTS:variorum_amd>)
    list(APPEND variorum_includes ${CMAKE_CURRENT_SOURCE_DIR}/AMD)
endif()

if(FORTRAN_FOUND)
    add_library(variorum_fortran OBJECT variorum.f90)
    list(APPEND variorum_sources $<TARGET_OBJECTS:variorum_fortran>)

    #####################################################
    # Setup install to copy the fortran modules
    #####################################################
    install(FILES
            ${CMAKE_Fortran_MODULE_DIRECTORY}/variorum.mod
            DESTINATION include)
endif()

if(BUILD_SHARED_LIBS)
    add_library(variorum SHARED
                ${variorum_sources}
                ${variorum_headers}
                ${variorum_deps})
else()
    add_library(variorum STATIC
                ${variorum_sources}
                ${variorum_headers}
                ${variorum_deps})
endif()

target_link_libraries(variorum PUBLIC ${HWLOC_LIBRARY})
target_link_libraries(variorum PUBLIC ${JANSSON_LIBRARY})
target_link_libraries(variorum PUBLIC m)
if(VARIORUM_WITH_NVIDIA)
target_link_libraries(variorum PUBLIC ${NVML_HEADER})
endif()
if(VARIORUM_WITH_AMD)
target_link_libraries(variorum PUBLIC ${ESMI_LIBRARY})
endif()

install(TARGETS variorum
        EXPORT  variorum
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION lib)

set(variorum_install_headers
    variorum.h
)

install(FILES ${variorum_install_headers}
        DESTINATION include)

include_directories(${variorum_includes})
