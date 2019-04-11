FROM alpine

RUN apk update && apk add \
    nodejs-current \
    build-base \
    go \
    rust \
    bash \
    python3 \
    pip3

RUN pip3 install cython

WORKDIR /home
COPY * /home/
ENTRYPOINT sh /home/run_tests.sh