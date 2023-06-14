cmake_minimum_required(VERSION 3.25)

project(stuff_ranvec_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        ranvec.cpp
        visualisation.cpp
        )

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::ranvec stf::qoi gtest gtest_main)
