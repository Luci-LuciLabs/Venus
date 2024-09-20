#SANDBOX CMAKE
cmake_minimum_required(VERSION 3.28)

#create sandbox exec. 
add_executable(Venus)

#set source subdirs
set(APP_DIR app)

#list all dir sources
file(GLOB_RECURSE APP_SOURCES ${APP_DIR}/*.cpp)

#add sources to app
target_sources(Venus PRIVATE ${APP_SOURCES})

#app header include directories
target_include_directories(Venus PRIVATE ${APP_DIR})

#link Venus_Engine to exec.
target_link_libraries(Venus PRIVATE Venus_Engine)

#include engine headers
target_include_directories(Venus PRIVATE ${CMAKE_SOURCE_DIR}/Venus/src/core)

#set output directories for packaged program
set_target_properties(Venus PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

#set RPATH to install path (this is the runtime path)
set_target_properties(Venus PROPERTIES
        INSTALL_RPATH "$ORIGIN"
)

#executable install rules
install(TARGETS Venus
        DESTINATION bin)