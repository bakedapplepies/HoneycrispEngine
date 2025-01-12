cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

add_subdirectory(scenes)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/Client/src/GameEntry.cpp
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/DefaultSceneTwo.cpp
    ${CMAKE_SOURCE_DIR}/Client/src/scenes/SpaceScene.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Client ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.Client PRIVATE
    ${CMAKE_SOURCE_DIR}/Client/src
)

target_link_libraries(${CMAKE_PROJECT_NAME}.Client PUBLIC ${CMAKE_PROJECT_NAME}.Client.Scenes)