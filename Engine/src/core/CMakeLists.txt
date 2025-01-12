cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/core/constants.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/core/Debug.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/core/Logging.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/core/FileSystem.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.Core ${SOURCE})