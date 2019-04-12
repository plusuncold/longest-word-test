FROM node:11-alpine

RUN wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt

RUN apk update && apk add \
    bash \
    build-base \
    go \
    python3 \
    python3-dev \
    rust

RUN pip3 install cython

WORKDIR /home
COPY * /home/
ENTRYPOINT sh /home/run_tests.sh