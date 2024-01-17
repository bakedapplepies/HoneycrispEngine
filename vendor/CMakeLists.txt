cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME}.Vendors CXX C)

# set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
# set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

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

add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(assimp)
add_subdirectory(fmt)
add_library(OPENFBX STATIC
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/libdeflate.c
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/libdeflate.h
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/ofbx.cpp
    ${CMAKE_SOURCE_DIR}/vendor/openfbx/src/ofbx.cpp
)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp/config.h")
    message("-- Building assimp")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/assimp
        COMMAND cmake . -G "Ninja"
    )
endif()