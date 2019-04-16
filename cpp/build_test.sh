#!/bin/bash

g++ -march=native -Ofast -fomit-frame-pointer -fopenmp main.cpp timer.cpp
