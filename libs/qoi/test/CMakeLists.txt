cmake_minimum_required(VERSION 3.25)

project(stuff_qoi_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        qoi.cpp
        test_images.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::qoi gtest gtest_main)
