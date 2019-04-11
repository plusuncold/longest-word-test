#!/bin/bash

echo "Get corpus"

if [ ! -f corpus.txt ]; then
    wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt
fi

echo "C++"

g++ -O3 main.cpp timer.cpp
./a.out


echo "Bash"
./bash.sh corpus.txt

echo "Python"
python3 test.py

echo "Rust"
rustc -O find_longest.rs
./find_longest corpus.txt

#echo "JS"
#node test.js
