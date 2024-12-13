FROM alpine:latest

RUN apk add --no-cache g++ make git

WORKDIR /app

RUN git clone https://github.com/s4sldvnkv/concurrent-shell-sort.git

RUN g++ ./concurrent-shell-sort/shell_sort.cpp -o shell_sort -fopenmp

CMD ./shell_sort > /results/result.txt
