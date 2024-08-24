# Copyright (c) 2024 Cryolite
# SPDX-License-Identifier: MIT
# This file is part of https://github.com/Cryolite/nyanten

cmake_minimum_required(VERSION 3.25.0)
project(nyanten)

enable_testing()

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()
message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
  add_compile_options(-Werror)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(NYANTEN_ENABLE_ASSERT ON)
endif()
if (NYANTEN_ENABLE_ASSERT)
  add_compile_definitions(NYANTEN_ENABLE_ASSERT)
endif()
message("NYANTEN_ENABLE_ASSERT=${NYANTEN_ENABLE_ASSERT}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    add_compile_definitions(_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC)
  endif()
endif()

if (NYANTEN_WITH_COVERAGE)
  add_compile_definitions(NYANTEN_WITH_COVERAGE)
  add_compile_options("-coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage")
endif()
message("NYANTEN_WITH_COVERAGE=${NYANTEN_WITH_COVERAGE}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    set(NYANTEN_WITH_ASAN ON)
  endif()
endif()
if (NYANTEN_WITH_ASAN)
  add_compile_options(-fsanitize=address)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()
message("NYANTEN_WITH_ASAN=${NYANTEN_WITH_ASAN}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
    set(NYANTEN_WITH_UBSAN ON)
  endif()
endif()
if (NYANTEN_WITH_UBSAN)
  add_compile_options(-fsanitize=undefined)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=undefined")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()
message("NYANTEN_WITH_UBSAN=${NYANTEN_WITH_UBSAN}")

if (NYANTEN_WITH_TSAN)
  add_compile_options(-fsanitize=thread)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=thread")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()
message("NYANTEN_WITH_TSAN=${NYANTEN_WITH_TSAN}")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_compile_options(-pthread)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

message("CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}")

if ((NOT DEFINED SHANTEN_NUMBER_ROOT) OR (SHANTEN_NUMBER_ROOT STREQUAL ""))
  message(FATAL_ERROR "`SHANTEN_NUMBER_ROOT` is not set.")
endif()
message("SHANTEN_NUMBER_ROOT=${SHANTEN_NUMBER_ROOT}")

include_directories(
  "${SHANTEN_NUMBER_ROOT}/src"
  .)

add_subdirectory(src)
add_subdirectory(test)
