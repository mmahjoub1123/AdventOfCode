#!/bin/bash
i="test"
if [[ "$1" == "all" ]]
then
    for filename in src/*; do
        exe_file=${filename%.cpp}
        input_file=inputs/${exe_file#src/}.txt
        rm -f $exe_file
        g++ -o $exe_file  $filename
        echo ${exe_file#src/}
        ./$exe_file $input_file
    done
else
    cpp_file="$1.cpp"
    input_file=inputs/$1.txt
    rm -f src/$1
    g++ -o "$1"  "src/$cpp_file"
    echo $1
    ./$1 $input_file
fi