# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(BUILD_DOCS ON CACHE BOOL "")
set(BUILD_TESTS ON CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_GPU OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM ON CACHE BOOL "")
set(VARIORUM_WITH_INTEL OFF CACHE BOOL "")

#SPHINX documentation building
set("SPHINX_EXECUTABLE" "/collab/usr/gapps/python/build/spack-coralea/opt/spack/linux-rhel7-ppc64le/gcc-4.8.5/py-sphinx-1.6.1-txjs2mkq6eoq6eteawczvpnnfopaychw/bin/sphinx-build" CACHE PATH "")
