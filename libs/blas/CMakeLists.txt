cmake_minimum_required(VERSION 3.25)

project(stuff_blas LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT TARGET gtest)
    add_subdirectory(../../thirdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest EXCLUDE_FROM_ALL)
endif()

if (NOT TARGET stuff_core)
    add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/libs/core)
endif()

if (NOT TARGET stuff_sfloat)
    add_subdirectory(../sfloat ${CMAKE_CURRENT_BINARY_DIR}/libs/sfloat)
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_core stuff_sfloat)

### TESTS

add_executable(${PROJECT_NAME}_tests
        test/matrix.cpp
        test/vector.cpp
        )

target_compile_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)
target_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} gtest gtest_main)