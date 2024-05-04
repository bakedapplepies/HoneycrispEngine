cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/managers/GLFWContext.h
    ${CMAKE_SOURCE_DIR}/src/managers/GLFWContext.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/RenderContext.h
    ${CMAKE_SOURCE_DIR}/src/managers/RenderContext.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/SceneManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/Texture2DManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/Texture2DManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/ShaderManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/ShaderManager.cpp
    ${CMAKE_SOURCE_DIR}/src/managers/ImGuiManager.h
    ${CMAKE_SOURCE_DIR}/src/managers/ImGuiManager.cpp
)

add_library(${CMAKE_PROJECT_NAME}.Managers ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)

target_precompile_headers(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Managers PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)