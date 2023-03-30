cmake_minimum_required(VERSION 3.25)

project(stuff_thread LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

if (NOT TARGET stuff_core)
    add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/libs/core)
endif()

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

target_link_libraries(${PROJECT_NAME} INTERFACE stuff_core)
