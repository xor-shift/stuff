cmake_minimum_required(VERSION 3.25)

project(stuff_elf_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        elf.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::elf gtest gtest_main)