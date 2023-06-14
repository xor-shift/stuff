cmake_minimum_required(VERSION 3.25)

project(stuff_core LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(stf::core ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
target_include_directories(${PROJECT_NAME} INTERFACE include)

if (STUFF_RUN_TESTS)
    add_subdirectory(test)
endif ()
