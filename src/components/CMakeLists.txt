cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/components/Transform.h
    ${CMAKE_SOURCE_DIR}/src/components/Transform.cpp
    ${CMAKE_SOURCE_DIR}/src/components/DrawData.h
    ${CMAKE_SOURCE_DIR}/src/components/Opacity.h
)

add_library(${CMAKE_PROJECT_NAME}.Components ${SOURCE})