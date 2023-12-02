include(FetchContent)
CPMAddPackage(
    NAME cli11
    GITHUB_REPOSITORY CLIUtils/CLI11
    GIT_TAG v2.3.2
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.12.0
  OPTIONS
    SPDLOG_BUILD_SHARED ON
    SPDLOG_BUILD_PIC ON
)

add_library(aoc_shared SHARED)
add_library(aoc::shared ALIAS aoc_shared)
target_include_directories(aoc_shared PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_sources(aoc_shared PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/app.cpp
)
target_link_libraries(aoc_shared PUBLIC
  CLI11::CLI11
  spdlog::spdlog
)
