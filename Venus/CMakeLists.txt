#ENGINE CMAKE
cmake_minimum_required(VERSION 3.28)

#create shared library of engine
add_library(Venus_Engine SHARED)
#library version
set_target_properties(Venus_Engine PROPERTIES
        VERSION 1.0 #version number
        SOVERSION 1 #majour version number
)

#set custom dirs
set(CORE_DIR src/core)

#list all dir sources
file(GLOB_RECURSE ENGINE_SOURCES ${CORE_DIR}/*.cpp)

#add sources to target
target_sources(Venus_Engine PRIVATE ${ENGINE_SOURCES})

#library header include directories
target_include_directories(Venus_Engine PUBLIC ${CORE_DIR})

#find and link glfw
find_package(glfw3 REQUIRED)
target_link_libraries(Venus_Engine PRIVATE glfw)
target_include_directories(Venus_Engine PRIVATE ${GLFW_INCLUDE_DIRS})

#set output directories for packaged program
set_target_properties(Venus_Engine PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

#set RPATH to install path (this is the runtime path)
set_target_properties(Venus_Engine PROPERTIES
        INSTALL_RPATH "$ORIGIN"
)

#install rules for library
install(TARGETS Venus_Engine
        EXPORT Venus_EngineExports
        DESTINATION bin)