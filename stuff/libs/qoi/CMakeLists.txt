cmake_minimum_required(VERSION 3.25)

project(stuff_qoi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT TARGET gtest)
    add_subdirectory(../../thirdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest EXCLUDE_FROM_ALL)
endif()

if (NOT TARGET stuff_core)
    add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/libs/core)
endif()

if (NOT TARGET stuff_bit)
    add_subdirectory(../bit ${CMAKE_CURRENT_BINARY_DIR}/libs/bit)
endif()

if (NOT TARGET stuff_expected)
    add_subdirectory(../expected ${CMAKE_CURRENT_BINARY_DIR}/libs/expected)
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_core stuff_bit stuff_expected)

### TESTS

add_executable(${PROJECT_NAME}_tests
        test/test_images.cpp

        test/qoi.cpp
        )

add_compile_options()

add_compile_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)
add_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} gtest gtest_main)

