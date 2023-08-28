#!/bin/sh

set -e

docker build -t vg .
docker run --rm --privileged -it vg