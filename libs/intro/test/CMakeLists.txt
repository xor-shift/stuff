cmake_minimum_required(VERSION 3.25)

project(stuff_intro_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        function.cpp
        intro.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::intro gtest gtest_main)
target_compile_options(${PROJECT_NAME} PRIVATE -ftemplate-backtrace-limit=128)
