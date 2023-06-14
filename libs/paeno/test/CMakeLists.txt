cmake_minimum_required(VERSION 3.25)

project(stuff_paeno_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        paeno.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::paeno gtest gtest_main)
