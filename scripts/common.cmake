function(sanitize_impl target type)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target} ${type} -fsanitize=address -fsanitize=undefined -O0 -g)
        target_link_options(${target} ${type} -fsanitize=address -fsanitize=undefined -O0 -g)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target} PUBLIC -ffast-math -march=native -mtune=native)
    endif ()

    if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        #target_compile_options(${target} PRIVATE -static-libsan)
        #target_link_options(${target} PRIVATE -static-libsan)
    else ()
        #target_compile_options(${target} PRIVATE -static-libasan)
        #target_link_options(${target} PRIVATE -static-libasan)
    endif ()
endfunction()

function(sanitize target)
    sanitize_impl(${target} PRIVATE)
endfunction()

function(optimize_or_sanitize target)
    sanitize(${PROJECT_NAME})
endfunction()
