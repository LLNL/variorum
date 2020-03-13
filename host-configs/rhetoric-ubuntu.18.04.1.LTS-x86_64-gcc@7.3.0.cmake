# c compiler
set(CMAKE_C_COMPILER "gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "gfortran" CACHE PATH "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS ON CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL ON CACHE BOOL "")

# path to global hwloc install
set(HWLOC_DIR "/usr/local/share/hwloc-1.11.7-install" CACHE PATH "")
