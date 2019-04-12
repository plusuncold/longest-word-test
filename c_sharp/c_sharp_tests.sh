#!/bin/bash

echo "Get corpus"

if [ ! -f corpus.txt ]; then
    wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt
fi

echo "C#"
echo "Select parallel"
dotnet LongestWord.dll corpus.txt

echo "Select non-parallel"
dotnet LongestWord.dll corpus.txt
