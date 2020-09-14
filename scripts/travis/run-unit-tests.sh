#!/bin/bash

cd build-travis
echo -e "PWD:" ${PWD}

# unit tests
env CTEST_OUTPUT_ON_FAILURE=1 make test

#
