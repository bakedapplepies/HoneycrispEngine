cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/serialize/Serializer.h
    ${CMAKE_SOURCE_DIR}/src/serialize/ModelSerializer.h
    ${CMAKE_SOURCE_DIR}/src/serialize/ModelSerializer.cpp
    ${CMAKE_SOURCE_DIR}/src/serialize/ImageSerializer.h
    ${CMAKE_SOURCE_DIR}/src/serialize/ImageSerializer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Serializer ${SOURCE})