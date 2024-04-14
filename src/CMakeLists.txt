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
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.h
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.h
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/binary_insert_delete.h

    # OpenGL stuff
    ${CMAKE_SOURCE_DIR}/src/opengl/VertexBuffer.h
    ${CMAKE_SOURCE_DIR}/src/opengl/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/ElementBuffer.h
    ${CMAKE_SOURCE_DIR}/src/opengl/ElementBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/VertexArray.h
    ${CMAKE_SOURCE_DIR}/src/opengl/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/Shader.h
    ${CMAKE_SOURCE_DIR}/src/opengl/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/Texture2D.h
    ${CMAKE_SOURCE_DIR}/src/opengl/Texture2D.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/TextureAtlas.h
    ${CMAKE_SOURCE_DIR}/src/opengl/TextureAtlas.cpp
    ${CMAKE_SOURCE_DIR}/src/opengl/UniformBuffer.h
    ${CMAKE_SOURCE_DIR}/src/opengl/UniformBuffer.cpp

    # managers
    ${CMAKE_SOURCE_DIR}/src/core/GLFWContext.h
    ${CMAKE_SOURCE_DIR}/src/core/GLFWContext.cpp
    ${CMAKE_SOURCE_DIR}/src/core/RenderContext.h
    ${CMAKE_SOURCE_DIR}/src/core/RenderContext.cpp
    ${CMAKE_SOURCE_DIR}/src/core/SceneManager.h
    ${CMAKE_SOURCE_DIR}/src/core/SceneManager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/Texture2DManager.h
    ${CMAKE_SOURCE_DIR}/src/core/Texture2DManager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/ShaderManager.h
    ${CMAKE_SOURCE_DIR}/src/core/ShaderManager.cpp
    ${CMAKE_SOURCE_DIR}/src/core/ImGuiManager.h
    ${CMAKE_SOURCE_DIR}/src/core/ImGuiManager.cpp

    # ecs
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentArray.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentCounter.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ComponentManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ecs/EntityManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/EntityManager.cpp
    ${CMAKE_SOURCE_DIR}/src/ecs/ECSTypes.h
    ${CMAKE_SOURCE_DIR}/src/ecs/System.h
    ${CMAKE_SOURCE_DIR}/src/ecs/SystemManager.h
    ${CMAKE_SOURCE_DIR}/src/ecs/SystemManager.cpp

    # types/components
    ${CMAKE_SOURCE_DIR}/src/components/Transform.h
    ${CMAKE_SOURCE_DIR}/src/components/Transform.cpp
    
    # systems
    ${CMAKE_SOURCE_DIR}/src/systems/Renderer.h
    ${CMAKE_SOURCE_DIR}/src/systems/Renderer.cpp
    
    # serializer
    ${CMAKE_SOURCE_DIR}/src/serialize/Serializer.h
    ${CMAKE_SOURCE_DIR}/src/serialize/ModelSerializer.h
    ${CMAKE_SOURCE_DIR}/src/serialize/ModelSerializer.cpp

    # Application stuff
    ${CMAKE_SOURCE_DIR}/src/core/Material.h
    ${CMAKE_SOURCE_DIR}/src/core/Material.cpp
    ${CMAKE_SOURCE_DIR}/src/Callbacks.h
    ${CMAKE_SOURCE_DIR}/src/Callbacks.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera.h
    ${CMAKE_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/Cube.h
    ${CMAKE_SOURCE_DIR}/src/Cube.cpp
    ${CMAKE_SOURCE_DIR}/src/DirectionalLight.h
    ${CMAKE_SOURCE_DIR}/src/DirectionalLight.cpp
    ${CMAKE_SOURCE_DIR}/src/PointLight.h
    ${CMAKE_SOURCE_DIR}/src/PointLight.cpp
    ${CMAKE_SOURCE_DIR}/src/SpotLight.h
    ${CMAKE_SOURCE_DIR}/src/SpotLight.cpp
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
    ${CMAKE_SOURCE_DIR}/src/scenes/SpaceScene.h
    ${CMAKE_SOURCE_DIR}/src/scenes/SpaceScene.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE}
    ${SCENES}
)
# -ffile-prefix-map=${CMAKE_SOURCE_DIR}=. messes up the debugger
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    # -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    # -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
    -Wall
    -Wextra
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-private-field
    # -Wno-
)

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD OUTPUT_VARIABLE COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    GLM_FORCE_SWIZZLE
    HNCRSP_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
    HNCRSP_GIT_COMMIT_ID="${COMMIT_ID}"
    HNCRSP_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
)
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
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GLAD
    glfw3
    IMGUI
    STB_IMAGE
    OPENFBX
    opengl32
    gdi32
    user32
    kernel32
    shell32
)
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmtd)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE assimpd)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE zlibstaticd)
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE assimp)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE zlibstatic)

endif()

# if (NOT EXISTS "${CMAKE_BINARY_DIR}/libassimp-5.dll")
#     message("-- Copying assimp-5.dll")
# endif()
# add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#     "${CMAKE_SOURCE_DIR}/lib/libassimp-5.dll"
#     $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
# )