#!/bin/bash
prefix="run.sh:"
./build.sh debug
if [[ $? -ne 0 ]]; then
    echo "$prefix run failed"
    echo "$prefix exiting"
    exit 1
fi
path="./bin/Debug/ChessBot"
echo "$prefix executing $path"
$path
exitcode=0
if [[ $# -eq 1 ]]; then
    if [[ $1 == "n" || $1 == "no" ]]; then
        echo "$prefix cleaning rejected"
        echo "$prefix exiting"
        exit $exitcode
    fi
fi
echo "$prefix cleaning"
make clean
cleaning="Makefile bin/* premake/build/* -r"
rm $cleaning
echo "$prefix exiting"
