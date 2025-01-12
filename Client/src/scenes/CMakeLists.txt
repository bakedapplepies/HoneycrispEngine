cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultScene.h
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultSceneTwo.h
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultSceneTwo.cpp
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/SpaceScene.h
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/SpaceScene.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Client.Scenes ${SOURCE})