cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/SceneManager.cpp
    ${PROJECT_SOURCE_DIR}/Texture2DManager.cpp
    ${PROJECT_SOURCE_DIR}/ShaderManager.cpp
    ${PROJECT_SOURCE_DIR}/ImGuiManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Managers ${SOURCE})