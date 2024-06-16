# Copyright (c) 2024 Cryolite. All rights reserved.
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

#!/usr/bin/env bash

set -euxo pipefail

g++ -o src/calsht.debug.o -c -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined ../shanten-number/src/calsht.cpp
g++ -o src/calsht.release.o -c -std=c++23 -I../shanten-number/src -O3 ../shanten-number/src/calsht.cpp

src/standard/build.sh

g++ -o src/test_correctness.debug -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined src/test_correctness.cpp src/calsht.debug.o
ASAN_OPTIONS=handle_abort=1 src/test_correctness.debug 1000000

g++ -o src/test_correctness.release -std=c++23 -I../shanten-number/src -O3 src/test_correctness.cpp src/calsht.release.o
src/test_correctness.release 100000000
