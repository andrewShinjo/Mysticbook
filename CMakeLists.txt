cmake_minimum_required(VERSION 3.10)
project(mysticbook)

# add_subdirectory processes the CMakeLists.txt inside the third party
# directory and add its libraries, executables, etc. to this build.
add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/SDL_ttf)
add_subdirectory(thirdparty/Unity)
add_subdirectory(src)
add_subdirectory(test)

