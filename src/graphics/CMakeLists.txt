cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
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
)

add_library(${CMAKE_PROJECT_NAME}.Graphics ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)

target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)