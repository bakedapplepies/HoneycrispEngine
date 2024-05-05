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

target_include_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)

target_precompile_headers(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)