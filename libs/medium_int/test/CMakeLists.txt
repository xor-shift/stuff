cmake_minimum_required(VERSION 3.25)

project(stuff_medium_int_tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        random.cpp

        carry.cpp
        tabled.cpp
        tabled_arithmetic.cpp
        random_arithmetic.cpp
        tabled_roundtrip.cpp
        random_roundtrip.cpp
)

sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::medium_int stf::random gtest gtest_main)
