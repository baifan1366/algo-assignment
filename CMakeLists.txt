cmake_minimum_required(VERSION 3.16)
project(CCP6214_Assignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(assignment_common
    src/common.cpp
)
target_include_directories(assignment_common PUBLIC include)

foreach(app
    dataset_generator
    radix_sort
    heap_sort
    hash_table_search
    radix_sort_step
    heap_sort_step
    hash_table_search_step
)
    add_executable(${app} src/${app}.cpp)
    target_link_libraries(${app} PRIVATE assignment_common)
endforeach()
