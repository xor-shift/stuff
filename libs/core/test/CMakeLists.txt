cmake_minimum_required(VERSION 3.25)

project(stuff_core_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        bunch_of_types.cpp
        stow.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::core gtest gtest_main)
