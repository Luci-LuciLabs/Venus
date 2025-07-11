cmake_policy(VERSION 3.28)
set(CMAKE_SYSTEM_NAME Windows)
find_program(MSVC_C NAMES cl REQUIRED)

if(MSVC_C)
set(CMAKE_C_COMPILER ${MSCV_C})
set(CMAKE_C_COMPILER ${MSVC_C})
endif()

set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)