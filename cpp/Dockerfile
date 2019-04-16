FROM alpine

WORKDIR /home

RUN apk update && apk add \
    build-base

RUN mkdir /home/corpus

COPY . /home
RUN g++ -march=native -Ofast -fomit-frame-pointer -fopenmp main.cpp timer.cpp
ENTRYPOINT /home/a.out /home/corpus/corpus.txt