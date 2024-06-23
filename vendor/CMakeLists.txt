cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME}.Vendors CXX C)

add_library(GLAD STATIC
    ${CMAKE_SOURCE_DIR}/vendor/glad/glad.c
)
target_include_directories(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/vendor)

add_library(IMGUI STATIC
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(IMGUI PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glfw/include)
# target_compile_definitions(IMGUI PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

add_library(STB_IMAGE STATIC
    ${CMAKE_SOURCE_DIR}/vendor/stb/stb_image.cpp
)
target_include_directories(STB_IMAGE PRIVATE ${CMAKE_SOURCE_DIR}/vendor)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_BUILD_TESTS OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL  "GLFW lib only" FORCE )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" FORCE )
add_subdirectory(glfw)

option( TRACY_ENABLE "" ON )
option( TRACY_ONLY_LOCALHOST "" ON )
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

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp/config.h")
    message("-- Building assimp")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/assimp
        COMMAND cmake . -G "Ninja"
    )
endif()
add_subdirectory(assimp)