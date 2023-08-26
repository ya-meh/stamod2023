#!/bin/sh

set -e

mkdir target || true
cd target
cmake ..
make
./statmod