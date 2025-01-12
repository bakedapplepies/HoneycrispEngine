cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Engine/src/ecs/ECSManager.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/ecs/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/Engine/src/ecs/SystemManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Engine.ECS ${SOURCE})