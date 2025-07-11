cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "kompressor" )

## Set our project name
project(${PROJECT_NAME})

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB SRCS "*.cpp" "*.c")

set(SRCS
    main.cpp
    datastructures/tree/tree.cpp
    compression_algorithms/huffman_compression/huffman_compression.cpp
    file_utils/file_utils.cpp
)

## Define the executable
add_executable(${PROJECT_NAME} ${SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
