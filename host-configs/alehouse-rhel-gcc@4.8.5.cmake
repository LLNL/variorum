# c compiler
set(CMAKE_C_COMPILER "gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "gfortran" CACHE PATH "")

set(BUILD_DOCS OFF CACHE PATH "")
set(BUILD_TESTING ON CACHE PATH "")
set(ENABLE_TESTS ON CACHE PATH "")

set(VARIORUM_WITH_AMD OFF CACHE PATH "")
set(VARIORUM_WITH_GPU OFF CACHE PATH "")
set(VARIORUM_WITH_IBM ON CACHE PATH "")
set(VARIORUM_WITH_INTEL OFF CACHE PATH "")

# path to global hwloc install
set(HWLOC_DIR "/usr/local/share/hwloc-1.11.7-install" CACHE PATH "")
