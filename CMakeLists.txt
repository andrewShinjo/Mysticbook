cmake_minimum_required(VERSION 3.16)
project(mysticbook)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/SDL_ttf EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(
  mysticbook 
  src/main.c
  src/gapbuffer/gapbuffer.c)

# Link to the actual SDL2 library.
target_link_libraries(mysticbook PRIVATE SDL2::SDL2 SDL2_ttf)
