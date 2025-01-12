cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/managers/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/managers/Texture2DManager.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/managers/ShaderManager.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/managers/ImGuiManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.Managers ${SOURCE})