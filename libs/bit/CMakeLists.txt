cmake_minimum_required(VERSION 3.25)

project(stuff_bit LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(stf::bit ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} INTERFACE stf::core)

if (STUFF_RUN_TESTS)
    add_subdirectory(test)
endif ()
