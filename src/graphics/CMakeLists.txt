cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/graphics/PostProcessingQueue.h
    ${CMAKE_SOURCE_DIR}/src/graphics/PostProcessingQueue.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Material.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Material.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/TextureAtlas.h
    ${CMAKE_SOURCE_DIR}/src/graphics/TextureAtlas.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Camera.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Cube.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Cube.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/DirectionalLight.h
    ${CMAKE_SOURCE_DIR}/src/graphics/DirectionalLight.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/PointLight.h
    ${CMAKE_SOURCE_DIR}/src/graphics/PointLight.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/SpotLight.h
    ${CMAKE_SOURCE_DIR}/src/graphics/SpotLight.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Cubemap.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Cubemap.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Mesh.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Model.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Model.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/Light.h
    ${CMAKE_SOURCE_DIR}/src/graphics/Light.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Graphics ${SOURCE})