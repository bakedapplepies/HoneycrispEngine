cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/VertexBuffer.cpp
    ${PROJECT_SOURCE_DIR}/ElementBuffer.cpp
    ${PROJECT_SOURCE_DIR}/VertexArray.cpp
    ${PROJECT_SOURCE_DIR}/Shader.cpp
    ${PROJECT_SOURCE_DIR}/Texture2D.cpp
    ${PROJECT_SOURCE_DIR}/Texture2DArray.cpp
    ${PROJECT_SOURCE_DIR}/Framebuffer.cpp
    ${PROJECT_SOURCE_DIR}/DepthMap.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Graphics.Core ${SOURCE})