cmake_minimum_required(VERSION 3.25)

project(stuff_ranvec LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT TARGET gtest)
    add_subdirectory(../../thirdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest EXCLUDE_FROM_ALL)
endif ()

if (NOT TARGET benchmark)
    add_subdirectory(../../thirdparty/benchmark ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/benchmark EXCLUDE_FROM_ALL)
endif ()

if (NOT TARGET stuff_random)
    add_subdirectory(../random ${CMAKE_CURRENT_BINARY_DIR}/libs/random)
endif ()

if (NOT TARGET stuff_blas)
    add_subdirectory(../blas ${CMAKE_CURRENT_BINARY_DIR}/libs/blas)
endif ()

if (NOT TARGET stuff_qoi)
    add_subdirectory(../qoi ${CMAKE_CURRENT_BINARY_DIR}/libs/qoi)
endif ()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_random stuff_blas)

### TESTS

add_executable(${PROJECT_NAME}_tests
        test/ranvec.cpp
        test/visualisation.cpp
        )

target_compile_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)
target_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} gtest gtest_main stuff_qoi)

### BENCHMARKS

add_executable(${PROJECT_NAME}_bench
        bench/bench.cpp
        )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}_bench PUBLIC -fsanitize=address -fsanitize=undefined)
    target_link_options(${PROJECT_NAME}_bench PUBLIC -fsanitize=address -fsanitize=undefined)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME}_bench PUBLIC -ffast-math -march=native -mtune=native)
endif()

target_link_libraries(${PROJECT_NAME}_bench ${PROJECT_NAME} benchmark benchmark_main)

