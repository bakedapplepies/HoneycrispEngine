cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/systems/Renderer.h
    ${CMAKE_SOURCE_DIR}/src/systems/Renderer.cpp
)

add_library(${CMAKE_PROJECT_NAME}.ECSSystems ${SOURCE})

# target_link_libraries(${CMAKE_PROJECT_NAME}.ECSSystems PRIVATE
#     Tracy::TracyClient
# )