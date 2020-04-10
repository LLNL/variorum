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
