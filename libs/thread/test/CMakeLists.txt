cmake_minimum_required(VERSION 3.25)

project(stuff_thread_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        channel.cpp
        )

#sanitize(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=thread)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=thread)

target_link_libraries(${PROJECT_NAME} stf::thread gtest gtest_main)
