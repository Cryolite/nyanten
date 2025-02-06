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
add_custom_target(src.standard.table
  ALL
  DEPENDS shupai_table.hpp zipai_table.hpp)

add_executable(src.standard.build_map
  build_map.cpp)
set_target_properties(src.standard.build_map
  PROPERTIES
  OUTPUT_NAME build_map)
add_dependencies(src.standard.build_map
  src.standard.table)
add_custom_command(
  OUTPUT shupai_map.hpp zipai_map.hpp
  COMMAND ASAN_OPTIONS=handle_abort=1 ./build_map shupai_map.hpp zipai_map.hpp
  COMMAND cp shupai_map.hpp "${CMAKE_SOURCE_DIR}/nyanten/standard/shupai_map.hpp"
  COMMAND cp zipai_map.hpp "${CMAKE_SOURCE_DIR}/nyanten/standard/zipai_map.hpp"
  DEPENDS src.standard.build_map)
add_custom_target(src.standard.map
  ALL
  DEPENDS shupai_map.hpp zipai_map.hpp)
