cmake_minimum_required(VERSION 3.25)

project(stuff_ply_tests LANGUAGES CXX)
project(stuff_ply_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        ply.cpp
        cow_ascii.cpp
        cow_binary.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::ply gtest gtest_main)
