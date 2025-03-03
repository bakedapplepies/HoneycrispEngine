cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

add_subdirectory(core)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/AxesCrosshair.cpp
    ${PROJECT_SOURCE_DIR}/Billboard.cpp
    ${PROJECT_SOURCE_DIR}/Camera.cpp
    ${PROJECT_SOURCE_DIR}/Cube.cpp
    ${PROJECT_SOURCE_DIR}/Cubemap.cpp
    ${PROJECT_SOURCE_DIR}/DepthPassCamera.cpp
    ${PROJECT_SOURCE_DIR}/DirectionalLight.cpp
    ${PROJECT_SOURCE_DIR}/PointLight.cpp
    ${PROJECT_SOURCE_DIR}/SpotLight.cpp
    ${PROJECT_SOURCE_DIR}/Material.cpp
    ${PROJECT_SOURCE_DIR}/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/Model.cpp
    ${PROJECT_SOURCE_DIR}/PostProcessingQueue.cpp
    ${PROJECT_SOURCE_DIR}/TextureAtlas.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Graphics ${SOURCE})

target_link_options(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_PROJECT_NAME}.Graphics.Core)