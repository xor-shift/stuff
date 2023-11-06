cmake_minimum_required(VERSION 3.25)

project(stuff_hash_bench LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        hash.cpp
)

optimize_or_sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::hash benchmark benchmark_main)
