cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/constants.cpp
    ${PROJECT_SOURCE_DIR}/Debug.cpp
    ${PROJECT_SOURCE_DIR}/Logging.cpp
    ${PROJECT_SOURCE_DIR}/FileSystem.cpp
    ${PROJECT_SOURCE_DIR}/Utils.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Core ${SOURCE})