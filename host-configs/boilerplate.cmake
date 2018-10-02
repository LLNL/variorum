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
set(BUILD_DOCS ON CACHE PATH "")

# build unit tests
set(BUILD_TESTING ON CACHE PATH "")
set(ENABLE_TESTS ON CACHE PATH "")

# build for specific architecture
set(VARIORUM_WITH_AMD OFF CACHE PATH "")
set(VARIORUM_WITH_GPU OFF CACHE PATH "")
set(VARIORUM_WITH_IBM OFF CACHE PATH "")
set(VARIORUM_WITH_INTEL ON CACHE PATH "")

# SPHINX documentation building
set("SPHINX_EXECUTABLE" "/path/to/sphinx-build" CACHE PATH "") 
