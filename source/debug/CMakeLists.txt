cmake_minimum_required(VERSION 3.28)
project(VN_logger LANGUAGES C CXX)



########################################################################
#               VENUS-LOGGER EXTERNAL DEPENDENCIES                 
########################################################################
set(quill_git_submodule_dir "${CMAKE_CURRENT_SOURCE_DIR}/.LIBRARIES/quill")
if(EXISTS "${quill_git_submodule_dir}")
  message(STATUS "Using git-submodule for quill dependency.")
  add_subdirectory("${quill_git_submodule_dir}")
else()
  message(STATUS "Using fetch-content for quill dependency.")
  include(FetchContent)
  FetchContent_Declare(quill
    GIT_REPOSITORY https://github.com/odygrd/quill.git
    GIT_TAG v10.0.0
  )
  FetchContent_MakeAvailable(quill)
endif()


########################################################################
#               VENUS-LOGGER SOURCE DIRECTORIES                 
########################################################################
set(vn_logger_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/logger")


########################################################################
#                     VENUS-LOGGER SOURCES                  
########################################################################
set(vn_logger_sources "${vn_logger_source_directory}/VN_logger.cpp")


########################################################################
#                   VENUS-LOGGER BUILD RULES                  
########################################################################
add_library(VN_logger STATIC ${vn_logger_sources})
set_target_properties(VN_logger PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(VN_logger PRIVATE quill)
target_precompile_headers(VN_logger PRIVATE "${vn_logger_source_directory}/quill_PCH.hpp")

target_compile_definitions(VN_logger PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(VN_logger PRIVATE
    $<$<CONFIG:Debug>:-Wall>
    $<$<CONFIG:Debug>:-Wextra>
    $<$<CONFIG:Debug>:-Werror>
    $<$<CONFIG:Debug>:-pedantic>
    $<$<CONFIG:Debug>:-ggdb>
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>
)

target_include_directories(VN_logger PUBLIC ${vn_logger_source_directory})

if(SANITIZE)
  target_compile_options(VN_logger PRIVATE ${SANITIZE_FLAGS})
  target_link_options(VN_logger PRIVATE ${SANITIZE_FLAGS})
endif()


