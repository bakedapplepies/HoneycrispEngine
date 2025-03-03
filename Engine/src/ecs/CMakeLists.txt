cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${PROJECT_SOURCE_DIR}/ECSManager.cpp
    ${PROJECT_SOURCE_DIR}/EntityManager.cpp
    ${PROJECT_SOURCE_DIR}/SystemManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.ECS ${SOURCE})