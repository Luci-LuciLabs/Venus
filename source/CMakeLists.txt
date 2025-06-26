cmake_minimum_required(VERSION 3.28)
project(Venus LANGUAGES C CXX)

########################################################################
#                       VENUS DIRECTORIES                 
########################################################################
set(application_source_directory "${CMAKE_CURRENT_SOURCE_DIR}/application")

########################################################################
#                         VENUS SOURCES               
########################################################################
set(application_sources "${application_source_directory}/application.cpp")


########################################################################
#                       VENUS BUILD RULES                
########################################################################
add_library(Venus SHARED ${application_sources})
set_target_properties(Venus PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(Venus PUBLIC ${application_source_directory})
target_link_libraries(Venus PUBLIC venusEngine)

target_compile_definitions(Venus PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(Venus PRIVATE
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
  target_compile_options(Venus PRIVATE ${SANITIZE_FLAGS})
  target_link_options(Venus PRIVATE ${SANITIZE_FLAGS})
endif()





########################################################################
#                           VENUS-CLIENT                
########################################################################
set(venus_client_sources "${CMAKE_CURRENT_SOURCE_DIR}/.client/main.cpp")

add_executable(V_client ${venus_client_sources})
target_link_libraries(V_client PRIVATE Venus)
target_include_directories(V_client PRIVATE ${application_source_directory})

target_compile_definitions(V_client PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(V_client PRIVATE
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
  target_compile_options(V_client PRIVATE ${SANITIZE_FLAGS})
  target_link_options(V_client PRIVATE ${SANITIZE_FLAGS})
endif()