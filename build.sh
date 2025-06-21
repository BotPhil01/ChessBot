#!/bin/bash
prefix="build.sh:"
echo "$prefix executing premake"
cd premake
./linpremake5 gmake
echo "$prefix executing make"
make -C build config=$1
if [[ $? -ne 0 ]]; then
    echo "$prefix build failure"
    echo "$prefix exiting"
    exit 1
fi
echo "$prefix build success"
echo "$prefix exiting"
exit 0
