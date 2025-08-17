cmake_minimum_required(VERSION 3.10)
enable_testing()

add_executable(
        mystic_editor_layout_test
        ${PROJECT_SOURCE_DIR}/src/editor/layout/mystic_editor_layout.c
        editor/layout/mystic_editor_layout_test.c)

target_include_directories(
        mystic_editor_layout_test
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/test)

target_link_libraries(mystic_editor_layout_test PRIVATE unity::framework)

add_test(NAME mystic_editor_layout_test COMMAND mystic_editor_layout_test)