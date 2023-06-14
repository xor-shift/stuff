cmake_minimum_required(VERSION 3.25)

project(stuff_expected_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        expected.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::expected gtest gtest_main)
