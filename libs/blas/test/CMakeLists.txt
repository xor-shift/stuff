cmake_minimum_required(VERSION 3.25)

project(stuff_blas_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        matrix.cpp
        vector.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::blas gtest gtest_main)
