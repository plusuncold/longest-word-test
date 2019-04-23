#!/bin/bash

echo "Running concurrent test"
time ./longest_word_finder concurrent

echo "Running single threaded test"
time ./longest_word_finder single_threaded
