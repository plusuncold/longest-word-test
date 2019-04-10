#!/bin/bash

echo "C++"

g++ -O3 main.cpp timer.cpp
./a.out


echo "Bash"
time ./bash.sh corpus.txt

echo "Python"
python3 test.py

echo "JS"
node test.js
