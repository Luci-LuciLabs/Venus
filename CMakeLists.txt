cmake_minimum_required(VERSION 3.28)
project(Venus LANGUAGES C CXX)

# Update project version here, will automatically apply to generated header for necessary version macros in source.
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # DO NOT CHANGE.

option(SANITIZE "Enables project sanitization, type is selected using presets." OFF)


set(cmake_helper_dir "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${cmake_helper_dir}/platforms.cmake")


if(SANITIZE)
  include("${cmake_helper_dir}/sanitize.cmake.in")
endif()

configurePlatform()

configure_file(
  ${cmake_helper_dir}/version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/source/common/version.hpp
)


# !!! PROJECT DIRECTORIES !!!
###########################################################################################
set(source_dir "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(common_dir "${source_dir}/common")
set(application_dir "${source_dir}/application")

set(glfw_context_dir "${source_dir}/glfwContext")
set(vulkan_core_dir "${source_dir}/vulkanCore")


# !!! PROJECT RULES !!!
###########################################################################################
# # !! PROJECT SOURCE !!
set(application_src "${application_dir}/application.cpp")
set(glfw_window_src "${glfw_context_dir}/window.cpp")
set(vulkan_instance_src "${vulkan_core_dir}/instance.cpp")
set(vulkan_core_src "${vulkan_core_dir}/vulkanCore.cpp")
set(vulkan_gpu_src "${vulkan_core_dir}/physicalDevice.cpp")
set(vulkan_logical_src "${vulkan_core_dir}/logicalDevice.cpp")

###############################################################

# # !! PROJECT TARGET !!
add_library(Venus SHARED
    ${application_src}
    ${glfw_window_src}
    ${vulkan_instance_src}
    ${vulkan_core_src}
    ${vulkan_gpu_src}
    ${vulkan_logical_src}
)
target_compile_definitions(Venus PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
if(NOT DISABLE_FLAGS)
  target_compile_options(Venus PRIVATE
      $<$<CONFIG:Debug>:-Wall>
      $<$<CONFIG:Debug>:-Wextra>
      $<$<CONFIG:Debug>:-Werror>
      $<$<CONFIG:Debug>:-pedantic>
      $<$<CONFIG:Debug>:-ggdb>
      $<$<CONFIG:Debug>:-fdiagnostics-color=always>
  )

  if(SANITIZE)
    target_compile_options(Venus PRIVATE ${SANITIZE_FLAGS})
    target_link_options(Venus PRIVATE ${SANITIZE_FLAGS})
  endif()
endif()

###############################################################
# # !! PROJECT INCLUDE !!
target_include_directories(Venus PUBLIC
    ${common_dir}
    ${logger_dir}
    ${application_dir}
    ${glfw_context_dir}
    ${vulkan_core_dir}
)
###############################################################







# # !! CLIENT RULES !!
set(client_dir "${source_dir}/.client")
set(client_src "${client_dir}/main.cpp")

add_executable(V_client ${client_src})
target_link_libraries(V_client PRIVATE Venus)
target_include_directories(V_client PRIVATE
  ${VENUS_INCLUDE_DIRS}
)

target_compile_definitions(V_client PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
if(NOT DISABLE_FLAGS)
  target_compile_options(V_client PRIVATE
      $<$<CONFIG:Debug>:-Wall>
      $<$<CONFIG:Debug>:-Wextra>
      $<$<CONFIG:Debug>:-Werror>
      $<$<CONFIG:Debug>:-pedantic>
      $<$<CONFIG:Debug>:-ggdb>
      $<$<CONFIG:Debug>:-fdiagnostics-color=always>
  )

  if(SANITIZE)
    target_compile_options(V_client PRIVATE ${SANITIZE_FLAGS})
    target_link_options(V_client PRIVATE ${SANITIZE_FLAGS})
  endif()
endif()

###############################################################




# !!! PROJECT DEPENDENCIES !!!
###########################################################################################

# # !! GLFW RULES !!
set(GLFW_BUILD_WAYLAND OFF)
set(GLFW_INSTALL OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(vendor/glfw)
target_link_libraries(Venus PRIVATE glfw)
target_include_directories(Venus PRIVATE ${GLFW_INCLUDE_DIRS})
###############################################################

# # !! VOLK RULES !!
add_subdirectory(vendor/volk)
target_link_libraries(Venus PRIVATE volk)
target_include_directories(Venus PRIVATE ${VOLK_INCLUDE_DIRS})
###############################################################

# # !! QUILL RULES !! 
add_subdirectory(vendor/quill)

set(logger_dir "${source_dir}/logger")
set(logger_src "${logger_dir}/VN_logger.cpp")

add_library(VN_logger STATIC "${logger_src}")
set_target_properties(VN_logger PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(VN_logger PUBLIC "${logger_dir}")
target_link_libraries(VN_logger PRIVATE quill)

target_precompile_headers(VN_logger PRIVATE "${logger_dir}/quill_PCH.hpp")

target_compile_definitions(VN_logger PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
if(NOT DISABLE_FLAGS)
  target_compile_options(VN_logger PRIVATE
      $<$<CONFIG:Debug>:-Wall>
      $<$<CONFIG:Debug>:-Wextra>
      $<$<CONFIG:Debug>:-Werror>
      $<$<CONFIG:Debug>:-pedantic>
      $<$<CONFIG:Debug>:-ggdb>
      $<$<CONFIG:Debug>:-fdiagnostics-color=always>
  )

  if(SANITIZE)
    target_compile_options(VN_logger PRIVATE ${SANITIZE_FLAGS})
    target_link_options(VN_logger PRIVATE ${SANITIZE_FLAGS})
  endif()
endif()


target_link_libraries(Venus PRIVATE VN_logger)