#!/bin/bash
prefix="run.sh:"
./build.sh tests 
if [[ $? -ne 0 ]]; then
    echo "$prefix run failed"
    echo "$prefix exiting"
    exit 1
fi
path="./bin/Tests/ChessBot"
echo "$prefix executing $path"

testPrefix="./tests/results/"
oldfile=$(ls $testPrefix | sort -g | tail -n 1)
newfile=$(echo "$(($oldfile + 1))")
$path | 
    while IFS= read -r line; do
        echo $line
        echo $line >> $testPrefix$newfile
    done

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
