#!/bin/bash

START=$(date +"%s%N")
< $1 tr ' ' '\n' | awk '{ if ( length >a ) { a = length; b=$0 }}END{ print "Length: "a"\nWord: "b }'
END=$(date +"%s%N")
let "NANO = $END - $START"
let "ELAPSE = $NANO / 100000"
echo "Time: "$ELAPSE"ms"
