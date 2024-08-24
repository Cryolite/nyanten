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
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" "${CMAKE_SOURCE_DIR}/map.bin" 100000000)
else()
  add_test(
    NAME correctness
    COMMAND env ASAN_OPTIONS=handle_abort=1 ./correctness "${SHANTEN_NUMBER_ROOT}" "${CMAKE_SOURCE_DIR}/map.bin" 10000)
endif()
