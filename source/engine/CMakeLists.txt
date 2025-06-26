cmake_minimum_required(VERSION 3.28)
project(venusEngine LANGUAGES C CXX)



########################################################################
#                   VENUS-ENGINE DIRECTORIES                 
########################################################################
set(engine_common_directory "${CMAKE_CURRENT_SOURCE_DIR}/common")

set(runtime_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/runtime")
set(runtime_subdirectories
        "${runtime_source_directory}/instance"
        "${runtime_source_directory}/window"
        "${runtime_source_directory}/input"
)

set(render_system_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/renderer")
set(render_system_directories
        "${render_system_source_directory}"
        "${render_system_source_directory}/device"
        "${render_system_source_directory}/swapchain"
        "${render_system_source_directory}/pipeline"
        "${render_system_source_directory}/renderer"
)

########################################################################
#                     VENUS-ENGINE SOURCES                  
########################################################################
set(runtime_sources 
        "${runtime_source_directory}/runtime.cpp"
        "${runtime_source_directory}/instance/instance.cpp"
        "${runtime_source_directory}/window/window.cpp"
        "${runtime_source_directory}/input/keyboardInput.cpp"
)

set(renderer_sources
        "${render_system_source_directory}/device/physicalDevice.cpp"
        "${render_system_source_directory}/device/logicalDevice.cpp"
        "${render_system_source_directory}/renderer/renderer.cpp"
        "${render_system_source_directory}/swapchain/swapchain.cpp"
        "${render_system_source_directory}/pipeline/graphicsPipeline.cpp"
)


set(venus_engine_sources
        ${runtime_sources}
        ${renderer_sources}
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
        ${runtime_subdirectories}
        ${render_system_directories}
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

