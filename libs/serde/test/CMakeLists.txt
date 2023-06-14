cmake_minimum_required(VERSION 3.25)

project(stuff_serde_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        serde.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::serde gtest gtest_main)
