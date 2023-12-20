cmake_minimum_required(VERSION 3.26.0)
project(LearnOpenGL.Deps CXX C)

add_library(GLAD STATIC
    ${CMAKE_SOURCE_DIR}/dependencies/glad/glad.c
)
target_include_directories(GLAD PRIVATE ${CMAKE_SOURCE_DIR}/dependencies)

add_library(FMT STATIC
    ${CMAKE_SOURCE_DIR}/dependencies/fmt/format.cc
)
target_include_directories(FMT PRIVATE ${CMAKE_SOURCE_DIR}/dependencies)

add_library(IMGUI STATIC
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(IMGUI PRIVATE ${CMAKE_SOURCE_DIR}/dependencies)
# target_compile_definitions(IMGUI PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

add_library(STB_IMAGE STATIC
    ${CMAKE_SOURCE_DIR}/dependencies/stb/stb_image.cpp
)
target_include_directories(STB_IMAGE PRIVATE ${CMAKE_SOURCE_DIR}/dependencies)