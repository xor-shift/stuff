cmake_minimum_required(VERSION 3.25)

project(stuff_qoi LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

option(STUFF_RUN_TESTS "" OFF)
option(STUFF_RUN_BENCHMARKS "" OFF)

if (NOT TARGET stuff_core)
    add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/libs/core)
endif ()

if (NOT TARGET stuff_bit)
    add_subdirectory(../bit ${CMAKE_CURRENT_BINARY_DIR}/libs/bit)
endif ()

if (NOT TARGET stuff_expected)
    add_subdirectory(../expected ${CMAKE_CURRENT_BINARY_DIR}/libs/expected)
endif ()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_core stuff_bit stuff_expected)

### TESTS

if (STUFF_RUN_TESTS)

    if (NOT TARGET gtest)
        add_subdirectory(../../thirdparty/googletest ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/googletest EXCLUDE_FROM_ALL)
    endif ()

    add_executable(${PROJECT_NAME}_tests
            test/test_images.cpp

            test/qoi.cpp
            )

    target_compile_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(${PROJECT_NAME}_tests PRIVATE -fsanitize=address -fsanitize=undefined)

    target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} gtest gtest_main)

endif ()

### BENCHMARKS

if (STUFF_RUN_BENCHMARKS)

    if (NOT TARGET benchmark)
        add_subdirectory(../../thirdparty/benchmark ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/benchmark EXCLUDE_FROM_ALL)
    endif ()

    add_executable(${PROJECT_NAME}_bench
            test/test_images.cpp

            bench/bench.cpp
            )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME}_bench PUBLIC -fsanitize=address -fsanitize=undefined)
        target_link_options(${PROJECT_NAME}_bench PUBLIC -fsanitize=address -fsanitize=undefined)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${PROJECT_NAME}_bench PUBLIC -ffast-math -march=native -mtune=native)
    endif ()

    target_link_libraries(${PROJECT_NAME}_bench ${PROJECT_NAME} benchmark benchmark_main)

endif ()
