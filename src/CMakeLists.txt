cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    # OpenGL stuff
    ${CMAKE_SOURCE_DIR}/src/core/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/core/ElementBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/core/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/core/UniformBuffer.cpp

    # types
    ${CMAKE_SOURCE_DIR}/src/types/Transform.cpp
    
    # utils
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.cpp

    # Application stuff
    ${CMAKE_SOURCE_DIR}/src/ImGuiController.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/Cube.cpp
    ${CMAKE_SOURCE_DIR}/src/Light.cpp
    ${CMAKE_SOURCE_DIR}/src/Cubemap.cpp
    ${CMAKE_SOURCE_DIR}/src/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/Model.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

set(SCENES
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultSceneTwo.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE}
    ${SCENES}
)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE GLM_FORCE_SWIZZLE)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/vendor/fmt)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GLAD
    glfw3
    fmtd
    IMGUI
    STB_IMAGE
    OPENFBX
    assimp.dll
    opengl32
    gdi32
    user32
    kernel32
    shell32
)

if (NOT EXISTS "${CMAKE_BINARY_DIR}/libassimp-5.dll")
    message("-- Copying assimp-5.dll")
endif()
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/lib/libassimp-5.dll" "${CMAKE_SOURCE_DIR}/build/vendor/fmt/bin/libfmtd.dll"
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)