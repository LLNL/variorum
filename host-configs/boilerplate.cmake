##################################
# empty  host-config
##################################
# insert compiler name here
##################################

# c compiler
set(CMAKE_C_COMPILER "/path/to/c_compiler" CACHE PATH "")

# cpp compiler
set(CMAKE_CXX_COMPILER "/path/to/cxx_compiler" CACHE PATH "")

# fortran compiler
set(CMAKE_Fortran_COMPILER  "/path/to/fortran_compiler" CACHE PATH "")

# build documentation (depends on sphinx, doxygen, and breathe)
set(BUILD_DOCS ON CACHE BOOL "")

# build unit tests
set(BUILD_TESTS ON CACHE BOOL "")

# build for specific architecture
set(VARIORUM_WITH_AMD OFF CACHE BOOL "")
set(VARIORUM_WITH_NVIDIA OFF CACHE BOOL "")
set(VARIORUM_WITH_IBM OFF CACHE BOOL "")
set(VARIORUM_WITH_INTEL ON CACHE BOOL "")

# SPHINX documentation building
set("SPHINX_EXECUTABLE" "/path/to/sphinx-build" CACHE PATH "")
