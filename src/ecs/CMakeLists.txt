cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentArray.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentCounter.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ecs/EntityManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSTypes.h
    ${CMAKE_SOURCE_DIR}/src/ecs/System.h
    ${CMAKE_SOURCE_DIR}/src/ecs/SystemManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/SystemManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.ECS ${SOURCE})