project(libubench)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)


add_library(ubench_static STATIC lib.c)
add_library(ubench_shared SHARED lib.c)
set_target_properties(ubench_static PROPERTIES OUTPUT_NAME ubench)
set_target_properties(ubench_shared PROPERTIES OUTPUT_NAME ubench)