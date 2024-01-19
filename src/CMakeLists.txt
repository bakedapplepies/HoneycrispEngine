cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    # utils
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.h
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/utils.h
    ${CMAKE_SOURCE_DIR}/src/utils/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.h
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.h
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.cpp

    # OpenGL stuff
    ${CMAKE_SOURCE_DIR}/src/renderer/VertexBuffer.h
    ${CMAKE_SOURCE_DIR}/src/renderer/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/renderer/ElementBuffer.h
    ${CMAKE_SOURCE_DIR}/src/renderer/ElementBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/renderer/VertexArray.h
    ${CMAKE_SOURCE_DIR}/src/renderer/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/renderer/Shader.h
    ${CMAKE_SOURCE_DIR}/src/renderer/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/renderer/Texture.h
    ${CMAKE_SOURCE_DIR}/src/renderer/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/renderer/UniformBuffer.h
    ${CMAKE_SOURCE_DIR}/src/renderer/UniformBuffer.cpp

    # managers
    ${CMAKE_SOURCE_DIR}/src/core/SceneManager.h
    ${CMAKE_SOURCE_DIR}/src/core/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Texture2DManager.h
    ${CMAKE_SOURCE_DIR}/src/core/Texture2DManager.cpp

    # types
    ${CMAKE_SOURCE_DIR}/src/types/Transform.h
    ${CMAKE_SOURCE_DIR}/src/types/Transform.cpp

    # Application stuff
    ${CMAKE_SOURCE_DIR}/src/ImGuiController.h
    ${CMAKE_SOURCE_DIR}/src/ImGuiController.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera.h
    ${CMAKE_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/Cube.h
    ${CMAKE_SOURCE_DIR}/src/Cube.cpp
    ${CMAKE_SOURCE_DIR}/src/Light.h
    ${CMAKE_SOURCE_DIR}/src/Light.cpp
    ${CMAKE_SOURCE_DIR}/src/Cubemap.h
    ${CMAKE_SOURCE_DIR}/src/Cubemap.cpp
    ${CMAKE_SOURCE_DIR}/src/Mesh.h
    ${CMAKE_SOURCE_DIR}/src/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/Model.h
    ${CMAKE_SOURCE_DIR}/src/Model.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.h
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/Window.h
    ${CMAKE_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

set(SCENES
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultScene.h
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultSceneTwo.h
    ${CMAKE_SOURCE_DIR}/src/scenes/DefaultSceneTwo.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE}
    ${SCENES}
)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    -Wall
    -Wextra
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-private-field
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE GLM_FORCE_SWIZZLE HNCRSP_PROJECT_DIR="${CMAKE_SOURCE_DIR}")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
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
    "${CMAKE_SOURCE_DIR}/lib/libassimp-5.dll"
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)