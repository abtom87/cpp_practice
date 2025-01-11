cmake_minimum_required(VERSION 3.0)

set(PROJ_NAME sample_boost)
project(${PROJ_NAME})

include_directories("/usr/local/lib/include/")

#add_compile_definitions(IP_ADDR="93.184.216.34" PORT=80)
add_compile_definitions(IP_ADDR="51.38.81.49" PORT=80)

# file(GLOB_RECURSE sources ./test_ObsDesign.cpp ./main/*.h)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


# add the data to the target, so it becomes visible in some IDE
add_executable(${PROJ_NAME} ${SOURCES})

# just for example add some compiler flags
target_compile_options(${PROJ_NAME}  PUBLIC -std=c++17 -Wall -Wfloat-conversion)