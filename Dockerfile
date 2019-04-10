FROM python:3-alpine

RUN apk update && apk add \
    nodejs \
    build-base

WORKDIR /home
COPY * /home/
# ENTRYPOINT sh /home/run_tests.sh