#!/bin/bash

echo "Get corpus"

if [ ! -f corpus.txt ]; then
    wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt
fi

echo "Bash"
./bash.sh corpus.txt

echo "Python"
python3 test.py

echo "JRuby"
jruby -J-Xmx15G test.jrb

echo "Cython"
python3 setup.py build_ext --inplace
python3 run_cythoned_test.py

echo "Rust"
cd ./find_longest_word/
cargo build --release
./target/release/find_longest_word corpus.txt
cd ..

echo "JS"
node --experimental-worker test_stream.js

echo "Golang"
go run ./timer.go

echo "C++"
cd cpp
./build_test.sh
./run_test.sh
cd ..

echo "Ruby"
ruby test.rb
