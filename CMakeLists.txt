cmake_minimum_required(VERSION 3.28)
project(Venus LANGUAGES C CXX)



set(app_src "${CMAKE_CURRENT_SOURCE_DIR}/source/app/application.cpp")
              
add_executable(Venus ${app_src})

add_subdirectory(source/renderSystem)
target_link_libraries(Venus PRIVATE renderSystem)
target_include_directories(Venus PRIVATE ${RENDERSYSTEM_INCLUDE_DIRS})