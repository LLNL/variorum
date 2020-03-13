# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(BUILD_DOCS ON CACHE BOOL "")
set(BUILD_TESTS ON CACHE BOOL "")

set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL ON CACHE BOOL "")

#SPHINX documentation building
set("SPHINX_EXECUTABLE" "/collab/usr/gapps/python/build/spack-toss3.2/opt/spack/linux-rhel7-x86_64/gcc-4.9.3/py-sphinx-1.6.3-pd3yayuuejdpggkyqvhxbljwh4fhpo43/bin/sphinx-build" CACHE PATH "")
