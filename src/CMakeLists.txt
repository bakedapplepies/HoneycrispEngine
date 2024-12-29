cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    # Application
    ${CMAKE_SOURCE_DIR}/src/constants.h
    ${CMAKE_SOURCE_DIR}/src/constants.cpp
    ${CMAKE_SOURCE_DIR}/src/Callbacks.h
    ${CMAKE_SOURCE_DIR}/src/Callbacks.cpp
    ${CMAKE_SOURCE_DIR}/src/Application.h
    ${CMAKE_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene.h
    ${CMAKE_SOURCE_DIR}/src/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/SceneRenderObj.h
    ${CMAKE_SOURCE_DIR}/src/SceneRenderObj.cpp
    ${CMAKE_SOURCE_DIR}/src/Window.h
    ${CMAKE_SOURCE_DIR}/src/Window.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/vendor/tracy/public/TracyClient.cpp
)
set(INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
    ${CMAKE_SOURCE_DIR}/vendor/tracy/public/tracy
)
set(LINK_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src
    ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt
    ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib
    ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib
)
set(PCH
    ${CMAKE_SOURCE_DIR}/src/pch/hncrsp_pch.h
    ${CMAKE_SOURCE_DIR}/src/pch/hncrsp_pch.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
    ${SOURCE}
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
)

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD OUTPUT_VARIABLE COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_compile_definitions(
    GLM_FORCE_SWIZZLE
    GLM_ENABLE_EXPERIMENTAL
    HNCRSP_TRACY_PROFILE
    HNCRSP_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
    HNCRSP_GIT_COMMIT_ID="${COMMIT_ID}"
    HNCRSP_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
)

add_subdirectory(managers)
add_subdirectory(graphics)
add_subdirectory(graphics/core)
add_subdirectory(utils)
add_subdirectory(ecs)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(serialize)
add_subdirectory(scenes)

# PCH ----------
target_precompile_headers(${CMAKE_PROJECT_NAME}               PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Utils         PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE ${PCH})
# ----------
# Include directories ----------
target_include_directories(${CMAKE_PROJECT_NAME}               PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Utils         PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE ${INCLUDE_DIRECTORIES})
# ----------
# Link directories ----------
target_link_directories(${CMAKE_PROJECT_NAME}               PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Utils         PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE ${LINK_DIRECTORIES})
# ----------
# Link Tracy ----------
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")  # for some reason Tracy raises an error in Debug mode
    target_link_libraries(${CMAKE_PROJECT_NAME}               PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Managers      PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Utils         PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.ECS           PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Components    PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE Tracy::TracyClient)
    target_link_libraries(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE Tracy::TracyClient)
endif()
# ----------

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GLAD
    IMGUI
    STB_IMAGE
    glfw
    fmt
    assimp
    zlibstatic
    ws2_32  # for tracy
    Dbghelp  # for tracy
    opengl32
    gdi32
    user32
    kernel32
    shell32

    ${CMAKE_PROJECT_NAME}.Managers
    ${CMAKE_PROJECT_NAME}.Graphics
    ${CMAKE_PROJECT_NAME}.Graphics.Core
    ${CMAKE_PROJECT_NAME}.Utils
    ${CMAKE_PROJECT_NAME}.ECS
    ${CMAKE_PROJECT_NAME}.Components
    ${CMAKE_PROJECT_NAME}.ECSSystems
    ${CMAKE_PROJECT_NAME}.Serializer
    ${CMAKE_PROJECT_NAME}.Scenes
)
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug") # ???
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw3d)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmtd)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE assimpd)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE zlibstaticd)
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw3)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE assimp)
    # target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE zlibstatic)

endif()

# Some post processing
# execute_process(
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
#     COMMAND python add_postprocessing_shaders.py ${CMAKE_BINARY_DIR}
# )