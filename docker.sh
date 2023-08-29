#!/bin/sh

set -e

docker build -t vg .
docker run --rm -it vg