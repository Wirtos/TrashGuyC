cmake_minimum_required(VERSION 3.10)
project(TrashGuyC C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(TrashGuyC main.c)

include(CheckLibraryExists)
check_library_exists(m pow "" LIBM_EXISTS)

if (LIBM_EXISTS)
    target_link_libraries(TrashGuyC PRIVATE m)
endif ()
