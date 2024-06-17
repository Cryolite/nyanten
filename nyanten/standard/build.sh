# Copyright (c) 2024 Cryolite. All rights reserved.
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

#!/usr/bin/env bash

set -euxo pipefail

mkdir -p build/standard

g++ -o build/standard/build_table -std=c++23 -D_GLIBCXX_DEBUG -Wall -g -fsanitize=address -fsanitize=undefined nyanten/standard/build_table.cpp
ASAN_OPTIONS=handle_abort=1 build/standard/build_table

g++ -o build/standard/test_hash -std=c++23 -D_GLIBCXX_DEBUG -Wall -g -fsanitize=address -fsanitize=undefined nyanten/standard/test_hash.cpp
ASAN_OPTIONS=handle_abort=1 build/standard/test_hash

g++ -o build/standard/build_map -std=c++23 -O3 nyanten/standard/build_map.cpp
build/standard/build_map
cat shupai_map.bin zipai_map.bin > map.bin
rm -f shupai_map.bin zipai_map.bin

g++ -o build/standard/test_correctness.debug -std=c++23 -D_GLIBCXX_DEBUG -I../shanten-number/src -Wall -g -fsanitize=address -fsanitize=undefined nyanten/standard/test_correctness.cpp build/calsht.debug.o
ASAN_OPTIONS=handle_abort=1 build/standard/test_correctness.debug 1000000

g++ -o build/standard/test_correctness.release -std=c++23 -I../shanten-number/src -O3 nyanten/standard/test_correctness.cpp build/calsht.release.o
build/standard/test_correctness.release 100000000

g++ -o build/standard/test_speed_empty -std=c++23 -I../shanten-number/src -O3 nyanten/standard/test_speed_empty.cpp
time build/standard/test_speed_empty 100000000

g++ -o build/standard/test_speed_baseline -std=c++23 -I../shanten-number/src -O3 nyanten/standard/test_speed_baseline.cpp build/calsht.release.o
time build/standard/test_speed_baseline 100000000

g++ -o build/standard/test_speed_proposed -std=c++23 -O3 nyanten/standard/test_speed_proposed.cpp
time build/standard/test_speed_proposed 100000000
