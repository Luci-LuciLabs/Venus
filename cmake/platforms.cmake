cmake_policy(VERSION 3.28)

function(configurePlatform)

  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "VENUS >>>> Building for Linux.")
  endif()
  
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "VENUS >>>> Building for Windows.")
  endif()
  
endfunction()