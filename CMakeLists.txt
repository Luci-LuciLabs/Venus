cmake_minimum_required(VERSION 3.28)
project(Venus LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
# Update project version here, will automatically apply to generated header for necessary version macros in source.
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

option(BUILD_DOCS "Build Doxygen documentation" OFF)
option(BUILD_DEBUG "Builds in Debug mode." OFF)
option(BUILD_RELEASE "Builds in Release mode." OFF)
option(BUILD_CLIENT "Builds and links to V_client." OFF)
option(DISABLE_FLAGS "Disables all compiler-flags regardless of build-type." OFF)
option(SANITIZE_PROJECT "Enables -fsanitize=address across entire project." OFF)

# this directory contains all cmake includes or files needing configuration by cmake
set(cmake_helper_dir "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# configure engine version header for instance class
configure_file(
  ${cmake_helper_dir}/version.hpp.in
  ${CMAKE_CURRENT_SOURCE_DIR}/source/common/version.hpp
)

include("${cmake_helper_dir}/platforms.cmake")
configurePlatform()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# !!! PROJECT DIRECTORIES !!!
###########################################################################################
set(source_dir "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(common_dir "${source_dir}/common")
set(window_dir "${source_dir}/window")
set(instance_dir "${source_dir}/instance")
set(application_dir "${source_dir}/application")
set(device_dir "${source_dir}/devices")

# !!! PROJECT RULES !!!
###########################################################################################
# # !! PROJECT SOURCE !!
set(window_src "${window_dir}/window.cpp")
set(instance_src "${instance_dir}/instance.cpp")
set(application_src "${application_dir}/application.cpp")
set(physicalDevice_src "${device_dir}/physicalDevice.cpp")

###############################################################

# # !! PROJECT TARGET !!
add_library(Venus SHARED
    ${logger_src}
    ${window_src}
    ${instance_src}
    ${application_src}
    ${physicalDevice_src}
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
      $<$<CONFIG:Release>:-03>
  )

  if(SANITIZE_PROJECT)
    target_compile_options(Venus PRIVATE -fsanitize=address)
    target_link_options(Venus PRIVATE -fsanitize=address)
  endif()
endif()

###############################################################
# # !! PROJECT INCLUDE !!
target_include_directories(Venus PUBLIC
    ${common_dir}
    ${logger_dir}
    ${window_dir}
    ${instance_dir}
    ${application_dir}
    ${device_dir}
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
      $<$<CONFIG:Release>:-03>
  )

  if(SANITIZE_PROJECT)
    target_compile_options(V_client PRIVATE -fsanitize=address)
    target_link_options(V_client PRIVATE -fsanitize=address)
  endif()
endif()

###############################################################




# !!! PROJECT DEPENDENCIES !!!
###########################################################################################

# # !! GLFW RULES !!
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
      $<$<CONFIG:Release>:-03>
  )

  if(SANITIZE_PROJECT)
    target_compile_options(VN_logger PRIVATE -fsanitize=address)
    target_link_options(VN_logger PRIVATE -fsanitize=address)
  endif()
endif()


target_link_libraries(Venus PRIVATE VN_logger)