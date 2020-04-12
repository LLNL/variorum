#!/bin/bash

echo -e "PWD:" ${PWD}

# test install (using with cmake example)
which cmake
cmake --version
cd install-travis/examples/using-with-cmake/c
echo -e "PWD:" ${PWD}
mkdir _test_build
cd _test_build
cmake ../
make VERBOSE=1
./variorum-print-power-example

# test install (using with make example)
cd ../../../using-with-make/c
echo -e "PWD:" ${PWD}
make VERBOSE=1
./variorum-print-power-example

# test install (using with make example)
cd ../../using-with-make/cpp
echo -e "PWD:" ${PWD}
make VERBOSE=1
./variorum-print-power-example

# test install (using with make example)
cd ../../using-with-make/fortran
echo -e "PWD:" ${PWD}
make VERBOSE=1
./ep.W.x

#
