cmake_minimum_required(VERSION 3.25)

project(stuff_paeno LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_core)

### TESTS

add_executable(${PROJECT_NAME}_tests
        test/paeno.cpp
        )

target_compile_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)
target_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)

add_subdirectory(../../thirdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest)
add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/libs/core)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} gtest gtest_main)
