cmake_minimum_required(VERSION 3.10)
project(mysticbook)

# add_subdirectory processes the CMakeLists.txt inside the third party
# directory and add its libraries, executables, etc. to this build.
add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/SDL_ttf)
add_subdirectory(thirdparty/Unity)

add_subdirectory(test)

# Creates an executable using the source files listed.
add_executable(${PROJECT_NAME}
        src/main.c
        src/mystic_color.c
        src/draw/mystic_draw.c)

# When building my app, link to these libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_ttf)
