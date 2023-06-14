cmake_minimum_required(VERSION 3.25)

project(stuff LANGUAGES CXX)

option(STUFF_RUN_TESTS "" OFF)
option(STUFF_RUN_BENCHMARKS "" OFF)

include(scripts/common.cmake)

# temporary
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

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
#add_subdirectory(libs/elf) # will be uncommented on the next commit :)
add_subdirectory(libs/expected)
add_subdirectory(libs/intro)
add_subdirectory(libs/paeno)
add_subdirectory(libs/qoi)
add_subdirectory(libs/random)
add_subdirectory(libs/ranvec)
add_subdirectory(libs/scope)
add_subdirectory(libs/serde)
add_subdirectory(libs/sfloat)
add_subdirectory(libs/thread)
