cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

add_subdirectory(scenes)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/TestClient/src/GameEntry.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultSceneTwo.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/SpaceScene.cpp
)

add_library(${CMAKE_PROJECT_NAME}.TestClient ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.TestClient PRIVATE
    ${CMAKE_SOURCE_DIR}/TestClient/src
)

target_link_libraries(${CMAKE_PROJECT_NAME}.TestClient PUBLIC ${CMAKE_PROJECT_NAME}.TestClient.Scenes)