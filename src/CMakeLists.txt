cmake_minimum_required(VERSION 3.10)

# Creates an executable using the source files listed.
add_executable(${PROJECT_NAME}
        main.c
        mystic_color.c
        draw/mystic_draw.c)

# Define a macro named RESOURCE_PATH that equals the path of the resource
# directory.
target_compile_definitions(${PROJECT_NAME} PRIVATE
        RESOURCE_PATH="${CMAKE_SOURCE_DIR}/resources")

# When building my app, link to these libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2 SDL2_ttf)