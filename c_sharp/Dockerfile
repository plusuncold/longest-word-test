FROM mcr.microsoft.com/dotnet/core/sdk:2.2
RUN wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt

WORKDIR /home
COPY * /home/
ENTRYPOINT sh /home/c_sharp_tests.sh