# Copyright (c) 2024 Cryolite. All rights reserved.
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

add_executable(src.standard.build_table
  build_table.cpp)
set_target_properties(src.standard.build_table
  PROPERTIES
  OUTPUT_NAME build_table)
add_custom_command(
  OUTPUT shupai_table.hpp zipai_table.hpp
  COMMAND ASAN_OPTIONS=handle_abort=1 ./build_table shupai_table.hpp zipai_table.hpp
  COMMAND cp shupai_table.hpp "${CMAKE_SOURCE_DIR}/nyanten/standard/shupai_table.hpp"
  COMMAND cp zipai_table.hpp "${CMAKE_SOURCE_DIR}/nyanten/standard/zipai_table.hpp"
  DEPENDS src.standard.build_table)
add_custom_target(src.standard.shupai_table
  DEPENDS shupai_table.hpp)
add_custom_target(src.standard.zipai_table
  DEPENDS zipai_table.hpp)

add_executable(src.standard.build_map
  build_map.cpp)
set_target_properties(src.standard.build_map
  PROPERTIES
  OUTPUT_NAME build_map)
add_dependencies(src.standard.build_map
  src.standard.shupai_table
  src.standard.zipai_table)
add_custom_command(
  OUTPUT map.bin
  COMMAND ASAN_OPTIONS=handle_abort=1 ./build_map shupai_map.bin zipai_map.bin
  COMMAND cat shupai_map.bin zipai_map.bin >map.bin
  COMMAND cp map.bin "${CMAKE_SOURCE_DIR}/map.bin"
  DEPENDS src.standard.build_map)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  add_custom_target(src.standard.map
    ALL
    DEPENDS map.bin)
else()
  add_custom_target(src.standard.map
    DEPENDS map.bin)
endif()
