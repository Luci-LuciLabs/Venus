cmake_minimum_required(VERSION 3.28)

project(Venus LANGUAGES CXX C)

set(CMAKE_BUILD_TYPE DEBUG)
set(POSITION_INDEPENDENT_CODE ON)

# set subdirectories
set(app_dir sources/app)
set(window_dir sources/coreVulkan/window)
set(volk_dir sources/coreVulkan/volk)
set(instance_dir sources/coreVulkan/instance)

# set sources
set(app_src ${app_dir}/app.cpp)
set(window_src ${window_dir}/glfwWindow.cpp)
set(volk_src ${volk_dir}/volk.c)
set(extensions_src ${instance_dir}/glfwExtensions.cpp)
set(layers_src ${instance_dir}/validationLayers.cpp)
set(instance_src ${instance_dir}/instance.cpp ${extensions_src} ${layers_src})

set(venus_src ${app_src} ${window_src} ${volk_src} ${instance_src})

# Venus library rules
add_library(Venus SHARED ${venus_src})
target_include_directories(Venus PUBLIC ${app_dir} ${window_dir} ${volk_dir} ${instance_dir})
target_compile_options(Venus PRIVATE -O2 -g -Wall -pedantic)


# test client rules
set(testclient_dir tools/client)
set(testclient_src ${testclient_dir}/testclient.cpp)
add_executable(clientTest ${testclient_src})
target_link_libraries(clientTest PRIVATE Venus)
target_compile_options(clientTest PRIVATE -O2 -g -Wall -pedantic)


# glfw rules
add_subdirectory(vendor/glfw)
target_link_libraries(Venus PRIVATE glfw)
target_include_directories(Venus PRIVATE ${GLFW_INCLUDE_DIRS})