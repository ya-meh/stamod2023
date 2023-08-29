FROM alpine:3.18.3

WORKDIR /app

RUN apk update && apk --no-cache add g++ make cmake valgrind qt5-qtbase-dev qt5-qtcharts-dev

COPY . /app

RUN rm -rf cmake-build* || true

RUN mkdir target && \
    cd target && \
    cmake .. && \
    make

ENTRYPOINT cd target && valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --suppressions=../.valgrind.supp ./statmod || sh