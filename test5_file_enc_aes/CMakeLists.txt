cmake_minimum_required(VERSION 3.0)

set(PROJ_NAME file_enc_aes)
project(${PROJ_NAME})

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filehandling.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/aescrypto.cpp
)

# add the data to the target, so it becomes visible in some IDE
add_executable(${PROJ_NAME} ${SOURCES})

target_link_libraries(${PROJ_NAME} PUBLIC -lcrypto)

# just for example add some compiler flags
target_compile_options(${PROJ_NAME} PUBLIC -std=c++17 -Wall -Wfloat-conversion)