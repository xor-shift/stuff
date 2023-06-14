cmake_minimum_required(VERSION 3.25)

project(stuff_scope_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        scope_guard.cpp
        unique_resource.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::scope gtest gtest_main)
