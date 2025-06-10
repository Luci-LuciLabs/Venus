cmake_minimum_required(VERSION 3.28)
project(venusEngine LANGUAGES C CXX)



########################################################################
#                   VENUS-ENGINE DIRECTORIES                 
########################################################################
set(engine_common_directory "${CMAKE_CURRENT_SOURCE_DIR}/common")

set(runtime_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/runtime")

set(rendering_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/rendering")

set(rendering_glfw_directory "${rendering_source_directory}/glfw")


set(rendering_vulkan_directory "${rendering_source_directory}/vulkan")
set(rendering_vulkan_directories_list
        "${rendering_vulkan_directory}"
        "${rendering_vulkan_directory}/instance"
        "${rendering_vulkan_directory}/device"
)

########################################################################
#                     VENUS-ENGINE SOURCES                  
########################################################################
set(runtime_sources "${runtime_source_directory}/runtime.cpp")

set(rendering_glfw_sources 
        "${rendering_glfw_directory}/window.cpp"
)

set(rendering_vulkan_sources
        "${rendering_vulkan_directory}/vulkanCore.cpp"
        "${rendering_vulkan_directory}/instance/instance.cpp"
        "${rendering_vulkan_directory}/device/physicalDevice.cpp"
        "${rendering_vulkan_directory}/device/logicalDevice.cpp"
)


set(venus_engine_sources
        ${runtime_sources}
        ${rendering_glfw_sources}
        ${rendering_vulkan_sources}
)





########################################################################
#                   VENUS-ENGINE BUILD RULES                  
########################################################################

add_library(venusEngine STATIC ${venus_engine_sources})
set_target_properties(venusEngine PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(venusEngine PUBLIC 
        ${engine_common_directory}
        ${runtime_source_directory}        
)

target_include_directories(venusEngine PRIVATE
        ${rendering_glfw_directory}
        ${rendering_vulkan_directories_list}
)

target_compile_definitions(venusEngine PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(venusEngine PRIVATE
    $<$<CONFIG:Debug>:-Wall>
    $<$<CONFIG:Debug>:-Wextra>
    $<$<CONFIG:Debug>:-Werror>
    $<$<CONFIG:Debug>:-pedantic>
    $<$<CONFIG:Debug>:-ggdb>
    $<$<CONFIG:Debug>:-fdiagnostics-color=always>

    $<$<CONFIG:Release>:-flto>
    $<$<CONFIG:Release>:-O2>
)

if(SANITIZE)
  target_compile_options(venusEngine PRIVATE ${SANITIZE_FLAGS})
  target_link_options(venusEngine PRIVATE ${SANITIZE_FLAGS})
endif()


target_link_libraries(venusEngine PUBLIC VN_logger)


add_subdirectory(.LIBRARIES/glfw)
target_link_libraries(venusEngine PRIVATE glfw)
target_include_directories(venusEngine PRIVATE ${GLFW_INCLUDE_DIRS})

add_subdirectory(.LIBRARIES/volk)
target_link_libraries(venusEngine PRIVATE volk)
target_include_directories(venusEngine PRIVATE ${VOLK_INCLUDE_DIRS})

