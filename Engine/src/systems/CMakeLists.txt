cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/Renderer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.ECSSystems ${SOURCE})