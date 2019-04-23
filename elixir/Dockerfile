FROM elixir:alpine

WORKDIR /home

COPY . /home

RUN mix local.hex --force y
RUN mix deps.get
RUN mix escript.build
ENTRYPOINT sh /home/run_test.sh