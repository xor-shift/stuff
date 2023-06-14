cmake_minimum_required(VERSION 3.25)

project(stuff_sfloat_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        soft_float.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::sfloat gtest gtest_main)
