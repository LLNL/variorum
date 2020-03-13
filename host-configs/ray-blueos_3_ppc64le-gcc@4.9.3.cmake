# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM ON CACHE BOOL "")
set(VARIORUM_WITH_INTEL OFF CACHE BOOL "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_TESTS ON CACHE BOOL "")
