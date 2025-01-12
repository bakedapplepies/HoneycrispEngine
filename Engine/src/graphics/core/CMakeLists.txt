cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/ElementBuffer.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/Shader.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/Texture2D.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/Texture2DArray.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/Framebuffer.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/core/DepthMap.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.Graphics.Core ${SOURCE})