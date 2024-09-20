#CMAKE MASTER FILE
cmake_minimum_required(VERSION 3.28)

#define project
Project(Venus-kit)

#standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add engine subdir
#add sandbox subdir
add_subdirectory(Venus)
add_subdirectory(Venus-Sandbox)

#set install dir for full package
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

#link exec. and engine lib
target_link_libraries(Venus PRIVATE Venus_Engine)

#install lib
install(TARGETS Venus_Engine
        DESTINATION bin)

#install executable
install(TARGETS Venus
        DESTINATION bin)
