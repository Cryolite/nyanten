# Copyright (c) 2024 Cryolite. All rights reserved.
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

#!/usr/bin/env bash

set -euxo pipefail

mkdir -p build

g++ -o build/calsht.debug.o -c -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined ../shanten-number/src/calsht.cpp
g++ -o build/calsht.release.o -c -std=c++23 -I../shanten-number/src -O3 ../shanten-number/src/calsht.cpp

nyanten/standard/build.sh

g++ -o build/test_correctness.debug -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined nyanten/test_correctness.cpp build/calsht.debug.o
ASAN_OPTIONS=handle_abort=1 build/test_correctness.debug 1000000

g++ -o build/test_correctness.release -std=c++23 -I../shanten-number/src -O3 nyanten/test_correctness.cpp build/calsht.release.o
build/test_correctness.release 100000000
