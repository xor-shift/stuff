cmake_minimum_required(VERSION 3.25)

project(stuff_ranvec_bench LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        bench.cpp
        )

optimize_or_sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::ranvec benchmark benchmark_main)
