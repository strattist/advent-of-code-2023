set(name day-2-cube-conundrum)
add_executable(${name} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_link_libraries(${name} PRIVATE
  aoc::shared
)
