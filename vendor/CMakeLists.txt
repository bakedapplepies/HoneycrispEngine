cmake_minimum_required(VERSION 3.26.0)
project(LearnOpenGL.Vendors CXX C)

add_subdirectory(glm)
add_subdirectory(assimp)
add_library(OPENFBX STATIC
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/libdeflate.c
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/libdeflate.h
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/ofbx.cpp
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/ofbx.cpp
)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp/config.h")
    message("-- Building assimp")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/assimp
        COMMAND cmake . -G "Ninja"
    )
endif()