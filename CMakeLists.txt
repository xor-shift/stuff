cmake_minimum_required(VERSION 3.25)
project(Tracer)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(stuff/libs/blas)
add_subdirectory(stuff/libs/qoi)
add_subdirectory(thirdparty/sfml)

add_executable(${PROJECT_NAME} main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")

#add_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fsanitize=undefined)
#add_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fsanitize=undefined)

target_link_libraries(${PROJECT_NAME}
        stuff_blas stuff_qoi
        SFML::System SFML::Window SFML::Graphics)
