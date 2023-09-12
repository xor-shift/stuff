cmake_minimum_required(VERSION 3.25)

project(stuff_stl_tests LANGUAGES CXX)
project(stuff_stl_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        stl.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::stl gtest gtest_main)
