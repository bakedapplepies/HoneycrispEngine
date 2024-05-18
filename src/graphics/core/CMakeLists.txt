cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/graphics/core/VertexBuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/ElementBuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/ElementBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/VertexArray.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Shader.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Texture2D.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Texture2D.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Texture2DArray.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Texture2DArray.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/UniformBuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/UniformBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Framebuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Framebuffer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Graphics.Core ${SOURCE})