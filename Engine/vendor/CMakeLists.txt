cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

add_library(GLAD STATIC
    ${PROJECT_SOURCE_DIR}/glad/glad.c
)
target_include_directories(GLAD PRIVATE ${PROJECT_SOURCE_DIR})

add_library(IMGUI STATIC
    ${PROJECT_SOURCE_DIR}/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(IMGUI PRIVATE ${PROJECT_SOURCE_DIR}/glfw/include)
# target_compile_definitions(IMGUI PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

add_library(STB_IMAGE STATIC
    ${PROJECT_SOURCE_DIR}/stb/stb_image.cpp
)
target_include_directories(STB_IMAGE PRIVATE ${PROJECT_SOURCE_DIR})

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" FORCE )
add_subdirectory(glfw)

option( TRACY_ENABLE "" ON )
# option( TRACY_ONLY_LOCALHOST "" ON )
add_subdirectory(tracy)

add_subdirectory(glm)
add_subdirectory(fmt)

set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/assimp/include/assimp/config.h")
    message("-- Building assimp")
    execute_process(
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/assimp
        COMMAND cmake . -G "Ninja"
    )
endif()
add_subdirectory(assimp)