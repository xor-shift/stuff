cmake_minimum_required(VERSION 3.25)

project(stuff LANGUAGES CXX)

option(STUFF_RUN_TESTS "" OFF)
option(STUFF_RUN_BENCHMARKS "" OFF)

include(scripts/common.cmake)

# temporary
#if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
#    add_compile_options(-stdlib=libc++)
#    add_link_options(-stdlib=libc++)
#endif ()

if (STUFF_RUN_TESTS)
    if (NOT TARGET gtest)
        add_subdirectory(thirdparty/googletest)
    endif ()
endif ()

if (STUFF_RUN_BENCHMARKS)
    if (NOT TARGET benchmark)
        add_subdirectory(thirdparty/benchmark)
    endif ()
endif ()

add_subdirectory(libs/bit)
add_subdirectory(libs/blas)
add_subdirectory(libs/core)
add_subdirectory(libs/elf)
add_subdirectory(libs/intro)
add_subdirectory(libs/hash)
add_subdirectory(libs/medium_int)
add_subdirectory(libs/paeno)
add_subdirectory(libs/ply)
add_subdirectory(libs/qoi)
add_subdirectory(libs/random)
add_subdirectory(libs/ranvec)
add_subdirectory(libs/scope)
add_subdirectory(libs/serde)
add_subdirectory(libs/sfloat)
add_subdirectory(libs/stl)
add_subdirectory(libs/thread)
