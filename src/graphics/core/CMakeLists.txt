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
    ${CMAKE_SOURCE_DIR}/src/graphics/core/UniformBuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/UniformBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Framebuffer.h
    ${CMAKE_SOURCE_DIR}/src/graphics/core/Framebuffer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Graphics.Core ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)

target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)