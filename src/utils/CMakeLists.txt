cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.h
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/utils.h
    ${CMAKE_SOURCE_DIR}/src/utils/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.h
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.h
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.h
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.h
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/binary_insert_delete.h
    ${CMAKE_SOURCE_DIR}/src/utils/Timer.h
    ${CMAKE_SOURCE_DIR}/src/utils/Timer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Utils ${SOURCE})