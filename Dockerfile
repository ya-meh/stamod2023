FROM alpine:latest

WORKDIR /app

RUN apk update && apk --no-cache add g++ make cmake valgrind qt5-qtbase-dev

COPY . /app

RUN rm -rf cmake-build* || true

RUN mkdir target && \
    cd target && \
    cmake .. && \
    make


ENTRYPOINT cd target && valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./statmod || sh