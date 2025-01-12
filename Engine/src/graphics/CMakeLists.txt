cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

add_subdirectory(core)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/AxesCrosshair.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Billboard.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Camera.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Cube.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Cubemap.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/DepthPassCamera.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/DirectionalLight.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/PointLight.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/SpotLight.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Material.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/Model.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/PostProcessingQueue.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/graphics/TextureAtlas.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.Graphics ${SOURCE})

target_link_options(${CMAKE_PROJECT_NAME}.Engine.Graphics PRIVATE ${CMAKE_PROJECT_NAME}.Engine.Graphics.Core)