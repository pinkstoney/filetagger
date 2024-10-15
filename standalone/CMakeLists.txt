cmake_minimum_required(VERSION 3.14...3.22)

project(FileTaggerStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME FileTagger SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Add dependencies ----
CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY onqtam/doctest
  VERSION 2.4.9
)

# ---- Specify Source Files Explicitly ----

set(STANDALONE_SOURCES
    source/main.cpp
)

add_executable(${PROJECT_NAME} ${STANDALONE_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "FileTagger")

# ---- Link dependencies ----
target_link_libraries(${PROJECT_NAME} PRIVATE FileTagger::FileTagger cxxopts doctest::doctest)
