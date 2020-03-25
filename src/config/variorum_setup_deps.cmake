include(CMakeFindDependencyMacro)

########################################################################
# Setup HWLOC
########################################################################
# If ZZZ_DIR not set, use known install path for HWLOC
if(NOT HWLOC_INCLUDE_LIB AND NOT HWLOC_LIB)
    set(HWLOC_LIB ${VARIORUM_HWLOC_LIB})
    set(HWLOC_INCLUDE_LIB ${VARIORUM_HWLOC_INCLUDE_DIR})
endif()

########################################################################
# Check for HWLOC_DIR
########################################################################
if(NOT HWLOC_INCLUDE_LIB AND NOT HWLOC_LIB)
    message(FATAL_ERROR "Could not find HWLOC. HWLOC requires HWLOC_LIB and HWLOC_INCLUDE_LIB.")
endif()

if(NOT EXISTS ${HWLOC_DIR}/lib/cmake/hwloc.cmake)
    message(FATAL_ERROR "Could not find HWLOC CMake include file (${HWLOC_DIR}/lib/cmake/hwloc.cmake)")
endif()

#######################################################################
# Import HWLOC's CMake targets
#######################################################################
find_dependency(HWLOC REQUIRED
                NO_DEFAULT_PATH
                PATHS ${HWLOC_DIR}/lib/cmake)
