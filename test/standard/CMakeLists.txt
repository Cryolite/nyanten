# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

add_executable(test.standard.hash
  hash.cpp)
set_target_properties(test.standard.hash
  PROPERTIES
  OUTPUT_NAME hash)
add_dependencies(test.standard.hash
  src.standard.shupai_table
  src.standard.zipai_table)
add_test(
  NAME standard/hash
  COMMAND env ASAN_OPTIONS=handle_abort=1 ./hash)

add_executable(test.standard.correctness
  correctness.cpp
  ${SHANTEN_NUMBER_ROOT}/src/calsht.cpp)
set_target_properties(test.standard.correctness
  PROPERTIES
  OUTPUT_NAME correctness)
add_dependencies(test.standard.correctness
  src.standard.map)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_test(
    NAME standard/correctness
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" "${CMAKE_SOURCE_DIR}/map.bin" 100000000)
else()
  add_test(
    NAME standard/correctness
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" "${CMAKE_SOURCE_DIR}/map.bin" 10000)
endif()

add_executable(test.standard.speed_empty
  speed_empty.cpp)
set_target_properties(test.standard.speed_empty
  PROPERTIES
  OUTPUT_NAME speed_empty)

add_executable(test.standard.speed_baseline
  speed_baseline.cpp
  ${SHANTEN_NUMBER_ROOT}/src/calsht.cpp)
set_target_properties(test.standard.speed_baseline
  PROPERTIES
  OUTPUT_NAME speed_baseline)

add_executable(test.standard.speed_proposed
  speed_proposed.cpp)
set_target_properties(test.standard.speed_proposed
  PROPERTIES
  OUTPUT_NAME speed_proposed)
