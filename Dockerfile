FROM alpine

RUN apk update && apk add \
    nodejs \
    build-base \
    go \
    rust \
    bash \
    python3

WORKDIR /home
COPY * /home/
ENTRYPOINT sh /home/run_tests.sh