# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

add_subdirectory(standard)

add_executable(test.correctness
  correctness.cpp
  ${SHANTEN_NUMBER_ROOT}/src/calsht.cpp)
set_target_properties(test.correctness
  PROPERTIES
  OUTPUT_NAME correctness)
add_dependencies(test.correctness
  src.standard.map)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_test(
    NAME correctness
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" 100000000)
else()
  add_test(
    NAME correctness
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" 10000)
endif()

add_executable(test.standard.performance_empty
  performance_empty.cpp)
set_target_properties(test.standard.performance_empty
  PROPERTIES
  OUTPUT_NAME performance_empty)

add_executable(test.standard.performance_baseline
  performance_baseline.cpp
  ${SHANTEN_NUMBER_ROOT}/src/calsht.cpp)
set_target_properties(test.standard.performance_baseline
  PROPERTIES
  OUTPUT_NAME performance_baseline)

add_executable(test.standard.performance_proposed
  performance_proposed.cpp)
set_target_properties(test.standard.performance_proposed
  PROPERTIES
  OUTPUT_NAME performance_proposed)
add_dependencies(test.standard.performance_proposed
  src.standard.keys)
