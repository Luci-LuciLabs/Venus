cmake_minimum_required(VERSION 3.28)
project(VenusProject LANGUAGES C CXX)

# Update project version here, will automatically apply to generated header for necessary version macros in source.
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_SUFFIX "alpha")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # DO NOT CHANGE.

option(SANITIZE "Enables project sanitization, type is selected using presets." OFF)


set(cmake_helper_dir "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(SANITIZE)
  include("${cmake_helper_dir}/sanitize.cmake.in")
endif()

configure_file(
  ${cmake_helper_dir}/version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/source/engine/common/version.hpp
)

add_subdirectory(source/debug)
add_subdirectory(source/engine)
add_subdirectory(source/)