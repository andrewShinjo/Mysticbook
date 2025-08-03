cmake_minimum_required(VERSION 3.10)

project(Mysticbook)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add SDL
add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)

# Add Unity testing framework
add_subdirectory(thirdparty/Unity)
include_directories(thirdparty/Unity/src)

# Create Unity library target
add_library(UnityLib STATIC thirdparty/Unity/src/unity.c)
target_include_directories(UnityLib PUBLIC thirdparty/Unity/src)

add_executable(
  Mysticbook 
  src/main.c
  src/draw/DrawWrapper.c
  src/window/WindowWrapper.c
)
target_link_libraries(Mysticbook PRIVATE UnityLib SDL3::SDL3)

# Add tests
add_subdirectory(test)
