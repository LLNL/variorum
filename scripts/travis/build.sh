#!/bin/bash

which g++
g++ --version
export CC=${COMPILER_CC}
export CXX=${COMPILER_CXX}
${CC} --version

cd $TRAVIS_BUILD_DIR
echo -e "TRAVIS_BUILD_DIR:" ${TRAVIS_BUILD_DIR}
echo -e "PWD:" ${PWD}

# install local spack
mkdir travis-spack-env
cd travis-spack-env
echo -e "PWD:" ${PWD}

# get specific version of spack
wget https://github.com/spack/spack/archive/v0.14.0.tar.gz
tar -xzf v0.14.0.tar.gz
mv spack-0.14.0 spack

export PATH=${PWD}/spack/bin:${PATH}

# setup mirror, so we don't have to download the file
spack mirror add local_filesystem file://${TRAVIS_BUILD_DIR}/scripts/travis/mirrors
ls ${TRAVIS_BUILD_DIR}/scripts/travis/mirrors

# hwloc
spack install hwloc
export HWLOC_DIR=`ls -d ${TRAVIS_BUILD_DIR}/travis-spack-env/spack/opt/spack/*/*/hwloc-*`
echo -e "HWLOC_DIR:" ${HWLOC_DIR}

which sphinx-build
cd ..

# create out-of-source build dir
mkdir build-travis
cd build-travis
echo -e "PWD:" ${PWD}

# cmake options
# build type
CMAKE_OPTS="-DCMAKE_BUILD_TYPE=Debug"
# shared or static libs
if [ "$BUILD_SHARED_LIBS" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DBUILD_SHARED_LIBS=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DBUILD_SHARED_LIBS=OFF"
fi
# variorum docs
if [ "$BUILD_DOCS" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DBUILD_DOCS=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DBUILD_DOCS=OFF"
fi
# variorum logging
if [ "$VARIORUM_LOG" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_LOG=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_LOG=OFF"
fi
# variorum architectures
if [ "$VARIORUM_WITH_INTEL" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_INTEL=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_INTEL=OFF"
fi
if [ "$VARIORUM_WITH_IBM" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_IBM=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_IBM=OFF"
fi
if [ "$VARIORUM_WITH_AMD" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_AMD=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_AMD=OFF"
fi
if [ "$VARIORUM_WITH_NVIDIA" == "ON" ]; then
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_NVIDIA=ON"
else
    CMAKE_OPTS="${CMAKE_OPTS} -DVARIORUM_WITH_NVIDIA=OFF"
fi

# install path
CMAKE_OPTS="${CMAKE_OPTS} -DCMAKE_INSTALL_PREFIX=../install-travis"
CMAKE_OPTS="${CMAKE_OPTS} -DHWLOC_DIR=${HWLOC_DIR}"

# confirm CMake config options
echo -e "CMake Options:" ${CMAKE_OPTS}

# make sure cmake is in our path
echo -e "PATH:" ${PATH}
which cmake
cmake --version
echo -e "PWD:" ${PWD}

# configure
cmake ${CMAKE_OPTS} ../src

# build, test, and install
make VERBOSE=1

#env CTEST_OUTPUT_ON_FAILURE=1 make test

#
