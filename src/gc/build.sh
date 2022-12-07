#!/usr/bin/env bash
# This will require that you have C and C++ toolchains, git,
# automake, autoconf, and libtool already installed.
if [ -z $_PROJECT_LOC ]; then
    echo "Must run setup first"
    exit 1
fi

# set up the repositories
cd $_PROJECT_LOC/src/gc

if [ ! -d $_PROJECT_LOC/src/gc/bdwgc ]; then
    git clone https://github.com/ivmai/bdwgc
else
    cd $_PROJECT_LOC/src/gc/bdwgc
    git pull
fi

# set up the build
cd $_PROJECT_LOC/src/gc/bdwgc && { echo $PWD; }
mkdir build || { echo "build already exists"; }
cd build

if [ "$_BUILD_TYPE" == "debug" ]; then
    cmake -Dbuild_tests=ON -DBUILD_SHARED_LIBS=OFF -Denable_gc_debug=ON ..
else
    cmake -Dbuild_tests=ON -DBUILD_SHARED_LIBS=OFF ..
fi

# Actually build it
cmake --build .

# test the build
ctest

# copy the results to where we can now use them
cp *.a $_PROJECT_LOC/src/lib/
cp ../include/gc/* $_PROJECT_LOC/src/include/gc
