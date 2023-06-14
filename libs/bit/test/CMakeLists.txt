cmake_minimum_required(VERSION 3.25)

project(stuff_bit_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        bit.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::bit gtest gtest_main)
