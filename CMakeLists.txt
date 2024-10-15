cmake_minimum_required(VERSION 3.14...3.22)

# Set CMake policies
cmake_policy(SET CMP0169 NEW)

# ---- Project ----

project(
  FileTagger
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please create a separate build directory and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----

include(cmake/CPM.cmake)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

# ---- Add dependencies ----
CPMAddPackage(
  NAME fmt
  GIT_TAG 10.2.1
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # Create an installable target
)

# ---- Find SQLite3 ----
find_package(SQLite3 REQUIRED)
if(NOT SQLite3_FOUND)
  message(FATAL_ERROR "SQLite3 not found. Please install SQLite3 development libraries.")
endif()

# ---- Add source and header files explicitly ----

set(HEADERS
    include/file-tagger/file-tagger.h
    include/file-tagger/file-scanner.h
    include/file-tagger/database-manager.h
)

set(SOURCES
    source/file-tagger.cpp
    source/file-scanner.cpp
    source/database-manager.cpp
)

# ---- Create library ----

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# Specify C++17 standard
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# Enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# ---- Link dependencies ----
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt ${SQLite3_LIBRARIES})

# ---- Specify include directories ----
target_include_directories(
  ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
    ${SQLite3_INCLUDE_DIRS}
)

# ---- Create an installable target ----

# Define the location for the version header
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 10.2.1"
)
