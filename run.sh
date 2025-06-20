#!/bin/bash
prefix="run.sh:"
./build.sh
if [[ $? -ne 0 ]]; then
    echo "$prefix run failed: exiting"
fi
path="./bin/Debug/ChessBot"
echo "$prefix executing $path"
./bin/Debug/ChessBot
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
