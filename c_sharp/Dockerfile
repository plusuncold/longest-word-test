FROM mcr.microsoft.com/dotnet/core/sdk:2.2
WORKDIR /home
RUN wget https://s3.wasabisys.com/correct-horse-battery-staple/trd/corpus.txt

COPY * /home/
ENTRYPOINT sh /home/c_sharp_tests.sh