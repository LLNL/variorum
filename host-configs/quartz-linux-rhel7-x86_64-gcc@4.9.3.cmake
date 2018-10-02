# c compiler
set(CMAKE_C_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/gcc" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/usr/tce/packages/gcc/gcc-4.9.3/bin/g++" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/usr/tce/packages/gcc/gcc-4.9.3/bin/gfortran" CACHE PATH "")

set(BUILD_DOCS ON CACHE PATH "")
set(BUILD_TESTING ON CACHE PATH "")
set(ENABLE_TESTS ON CACHE PATH "")

#SPHINX documentation building
set("SPHINX_EXECUTABLE" "/collab/usr/gapps/python/build/spack-toss3.2/opt/spack/linux-rhel7-x86_64/gcc-4.9.3/py-sphinx-1.6.3-pd3yayuuejdpggkyqvhxbljwh4fhpo43/bin/sphinx-build" CACHE PATH "")
