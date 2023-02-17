cmake_minimum_required(VERSION 3.25)

project(stuff_core LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)
