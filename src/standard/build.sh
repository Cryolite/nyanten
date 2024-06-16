# Copyright (c) 2024 Cryolite. All rights reserved.
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

#!/usr/bin/env bash

set -euxo pipefail

g++ -o src/standard/build_table -std=c++23 -D_GLIBCXX_DEBUG -Wall -g -fsanitize=address -fsanitize=undefined src/standard/build_table.cpp
ASAN_OPTIONS=handle_abort=1 src/standard/build_table

g++ -o src/standard/test_hash -std=c++23 -D_GLIBCXX_DEBUG -Wall -g -fsanitize=address -fsanitize=undefined src/standard/test_hash.cpp
ASAN_OPTIONS=handle_abort=1 src/standard/test_hash

g++ -o src/standard/build_map -std=c++23 -O3 src/standard/build_map.cpp
src/standard/build_map
cat src/standard/shupai_map.bin src/standard/zipai_map.bin > map.bin
rm -f src/standard/shupai_map.bin src/standard/zipai_map.bin

g++ -o src/standard/test_correctness.debug -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined src/standard/test_correctness.cpp src/calsht.debug.o
ASAN_OPTIONS=handle_abort=1 src/standard/test_correctness.debug 1000000

g++ -o src/standard/test_correctness.release -std=c++23 -I../shanten-number/src -O3 src/standard/test_correctness.cpp src/calsht.release.o
src/standard/test_correctness.release 100000000

g++ -o src/standard/test_speed_empty -std=c++23 -I../shanten-number/src -O3 src/standard/test_speed_empty.cpp
time src/standard/test_speed_empty 100000000

g++ -o src/standard/test_speed_baseline -std=c++23 -I../shanten-number/src -O3 src/standard/test_speed_baseline.cpp src/calsht.release.o
time src/standard/test_speed_baseline 100000000

g++ -o src/standard/test_speed_proposed -std=c++23 -O3 src/standard/test_speed_proposed.cpp
time src/standard/test_speed_proposed 100000000
