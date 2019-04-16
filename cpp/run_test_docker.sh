#!/bin/bash

docker run --rm -v $(pwd)/..:/home/corpus:z -ti cpp
