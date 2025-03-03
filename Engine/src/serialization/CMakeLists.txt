cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/ModelSerializer.cpp
    ${PROJECT_SOURCE_DIR}/ImageSerializer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Serialization ${SOURCE})