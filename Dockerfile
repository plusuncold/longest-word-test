FROM node:11-alpine

WORKDIR /home
RUN wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt

RUN apk update && apk add \
    bash \
    build-base \
    go \
    python3 \
    python3-dev \
    rustc \
	cargo

RUN pip3 install cython

COPY * /home/
ENTRYPOINT sh /home/run_tests.sh
