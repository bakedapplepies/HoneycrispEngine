cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/WindowHandler.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/RenderContext.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/Callbacks.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/GameLoop.cpp
    ${CMAKE_SOURCE_DIR}/vendor/tracy/public/TracyClient.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.Source ${SOURCE} ${CMAKE_SOURCE_DIR}/Engine/src/main.cpp)

add_subdirectory(managers)
add_subdirectory(graphics)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(serialize)

target_link_libraries(${CMAKE_PROJECT_NAME}.Engine.Source PUBLIC
    ${CMAKE_PROJECT_NAME}.Engine.Managers
    ${CMAKE_PROJECT_NAME}.Engine.Graphics
    ${CMAKE_PROJECT_NAME}.Engine.Graphics.Core
    ${CMAKE_PROJECT_NAME}.Engine.Core
    ${CMAKE_PROJECT_NAME}.Engine.ECS
    ${CMAKE_PROJECT_NAME}.Engine.Components
    ${CMAKE_PROJECT_NAME}.Engine.ECSSystems
    ${CMAKE_PROJECT_NAME}.Engine.Serializer
)