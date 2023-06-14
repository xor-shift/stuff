cmake_minimum_required(VERSION 3.25)

project(stuff_random_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        normal.cpp
        random.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::random gtest gtest_main)
