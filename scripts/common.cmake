function(sanitize target)
    target_compile_options(${target} PRIVATE -fsanitize=address -fsanitize=undefined)
    target_link_options(${target} PRIVATE -fsanitize=address -fsanitize=undefined)

    if (${CMAKE_CXX_COMPILER_ID} EQUAL "Clang")
        target_compile_options(${target} PRIVATE -static-libsan)
        target_link_options(${target} PRIVATE -static-libsan)
    else ()
        target_compile_options(${target} PRIVATE -static-libasan)
        target_link_options(${target} PRIVATE -static-libasan)
    endif ()
endfunction()

function(optimize_or_sanitize target)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        sanitize(${PROJECT_NAME})
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target} PUBLIC -ffast-math -march=native -mtune=native)
    endif ()
endfunction()