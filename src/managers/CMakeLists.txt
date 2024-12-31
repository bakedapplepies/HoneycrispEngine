cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/managers/WindowHandler.h
    ${CMAKE_SOURCE_DIR}/src/managers/WindowHandler.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/RenderContext.h
    ${CMAKE_SOURCE_DIR}/src/managers/RenderContext.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/SceneManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/Texture2DManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/Texture2DManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/ShaderManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/ShaderManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/ImGuiManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/ImGuiManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Managers ${SOURCE})