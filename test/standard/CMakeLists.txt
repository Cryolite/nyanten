# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

add_executable(test.standard.hash
  hash.cpp)
set_target_properties(test.standard.hash
  PROPERTIES
  OUTPUT_NAME hash)
add_dependencies(test.standard.hash
  src.standard.table)
add_test(
  NAME standard/hash
  COMMAND env ASAN_OPTIONS=handle_abort=1 ./hash)
