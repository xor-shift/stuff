cmake_minimum_required(VERSION 3.25)

project(stuff_qoi_bench LANGUAGES CXX)

add_executable(${PROJECT_NAME}
        bench.cpp
        ../test/test_images.cpp
        )

optimize_or_sanitize(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} stf::qoi benchmark benchmark_main)
