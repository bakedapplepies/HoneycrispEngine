cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/WindowHandler.cpp
    ${PROJECT_SOURCE_DIR}/RenderContext.cpp
    ${PROJECT_SOURCE_DIR}/Callbacks.cpp
    ${PROJECT_SOURCE_DIR}/Application.cpp
    ${PROJECT_SOURCE_DIR}/Scene.cpp
    ${PROJECT_SOURCE_DIR}/GameLoop.cpp
    ${PROJECT_SOURCE_DIR}/../vendor/tracy/public/TracyClient.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Source ${SOURCE} ${PROJECT_SOURCE_DIR}/main.cpp)

add_subdirectory(managers)
add_subdirectory(graphics)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(serialization)

target_link_libraries(${CMAKE_PROJECT_NAME}.Source PUBLIC
    ${CMAKE_PROJECT_NAME}.Managers
    ${CMAKE_PROJECT_NAME}.Graphics
    ${CMAKE_PROJECT_NAME}.Graphics.Core
    ${CMAKE_PROJECT_NAME}.Core
    ${CMAKE_PROJECT_NAME}.ECS
    ${CMAKE_PROJECT_NAME}.Components
    ${CMAKE_PROJECT_NAME}.ECSSystems
    ${CMAKE_PROJECT_NAME}.Serialization
)