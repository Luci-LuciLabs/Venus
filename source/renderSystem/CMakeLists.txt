cmake_minimum_required(VERSION 3.28)
project(Venus_rendersytem LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE "Debug")

#    RENDERSYSTEM RULES
## !!! PROJECT DIRECTORIES !!!
##############################################################################################################
set(common_dir "${CMAKE_CURRENT_SOURCE_DIR}/common")
set(instance_dir "${CMAKE_CURRENT_SOURCE_DIR}/instance")
set(window_dir "${CMAKE_CURRENT_SOURCE_DIR}/window")
set(device_dir "${CMAKE_CURRENT_SOURCE_DIR}/device")
set(swapchain_dir "${CMAKE_CURRENT_SOURCE_DIR}/swapchain")

set(render_dir "${CMAKE_CURRENT_SOURCE_DIR}/renderSystem")

## !!! PROJECT SOURCES !!!
##############################################################################################################
set(instance_src "${instance_dir}/instance.cpp")
set(window_src "${window_dir}/window.cpp")
set(physical_device_src "${device_dir}/physicalDevice.cpp")
set(logical_device_src "${device_dir}/logicalDevice.cpp")
set(swapchain_src "${swapchain_dir}/swapchain.cpp")


set(render_src "${render_dir}/renderSystem.cpp")

set(renderSystem_sources ${instance_src}
                         ${window_src} 
                          ${render_src}
                          ${physical_device_src}
                          ${logical_device_src}
                          ${swapchain_src})

## !!! PROJECT TARGET !!!
##############################################################################################################
add_library(renderSystem ${renderSystem_sources})
target_include_directories(renderSystem PUBLIC ${instance_dir}
                                               ${window_dir}
                                               ${render_dir}
                                               ${device_dir}
                                               ${common_dir}
                                               ${swapchain_dir})

target_compile_options(renderSystem PRIVATE -g -ggdb -Wall -Wextra -pedantic -fdiagnostics-color=always)

#       VOLK RULES
## !!! CORE DEPENDENCY !!!
##############################################################################################################
add_subdirectory(.vendor/volk)
target_link_libraries(renderSystem PRIVATE volk)
target_include_directories(renderSystem PRIVATE ${VOLK_INCLUDE_DIRS})

#       GLFW RULES
## !!! CORE DEPENDENCY !!!
##############################################################################################################
add_subdirectory(.vendor/glfw)
target_link_libraries(renderSystem PRIVATE glfw)
target_include_directories(renderSystem PRIVATE ${GLFW_INCLUDE_DIRS})