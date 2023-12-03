cmake_minimum_required(VERSION 3.22)

project(aoc23 CXX)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ Standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION ON)

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

add_subdirectory(${CMAKE_SOURCE_DIR}/shared)
add_subdirectory(${CMAKE_SOURCE_DIR}/day-1-trebuchet)
add_subdirectory(${CMAKE_SOURCE_DIR}/day-2-cube-conundrum)
add_subdirectory(${CMAKE_SOURCE_DIR}/day-3-gear-ratios)
