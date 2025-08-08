cmake_minimum_required(VERSION 4.0.1)
set(CMAKE_MODULE_PATH "/Users/ashinjo/Documents/AndyCodebase/app/Mysticbook/thirdparty/SDL/cmake")
project(CheckOBJC OBJC)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/result.cmake"
  "set(CMAKE_OBJC_COMPILER \"${CMAKE_OBJC_COMPILER}\")\n"
  
  )