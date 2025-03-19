cmake_minimum_required(VERSION 3.26.0)
project(HoneycrispEngine CXX C)

add_subdirectory(vendor)

# Find packages
find_package(Git REQUIRED)

# Build-type definitions
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    add_compile_definitions(HNCRSP_RELEASE)
endif()

add_compile_definitions(
    GLM_FORCE_SWIZZLE
    GLM_ENABLE_EXPERIMENTAL
    HNCRSP_TRACY_PROFILE
    HNCRSP_PROJECT_DIR="${CMAKE_SOURCE_DIR}"
    HNCRSP_GIT_COMMIT_ID="${COMMIT_ID}"
    HNCRSP_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
)

set(INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor
    ${GLFW_INSTALL_DIR}/install/include
    ${GLM_INSTALL_DIR}/install/include
    ${ASSIMP_INSTALL_DIR}/install/include
    # ${ASSIMP_INSTALL_DIR}/install/include/include/assimp
    # ${ASSIMP_INSTALL_DIR}/install/include/assimp-build/include
    ${FMT_INSTALL_DIR}/install/include
    ${TRACY_INSTALL_DIR}/install/include/tracy
)
set(LINK_DIRECTORIES
    ${GLFW_INSTALL_DIR}/install/lib
    ${GLM_INSTALL_DIR}/install/lib
    ${FMT_INSTALL_DIR}/install/lib
    ${ASSIMP_INSTALL_DIR}/install/lib
    ${TRACY_INSTALL_DIR}/install/lib
)
set(PCH
    ${PROJECT_SOURCE_DIR}/include/api/pch/hncrsp_pch.h
)
set(DEFINES
    HNCRSP_ENGINE
)

add_library(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Engine/src/main.cpp)

add_dependencies(${CMAKE_PROJECT_NAME}
    GLFW_EXTERN
    GLM_EXTERN
    FMT_EXTERN
    ASSIMP_EXTERN
    TRACY_EXTERN
)

# -ffile-prefix-map=${CMAKE_SOURCE_DIR}=. messes up the debugger
set(COMPILE_OPTIONS
    # -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    # -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
    -Wall
    -Wextra
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-private-field
)

# Subdirectories
add_subdirectory(src)

target_compile_options(${CMAKE_PROJECT_NAME}.Source PRIVATE
    ${COMPILE_OPTIONS}    
)

# Set macro as git commit id
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD OUTPUT_VARIABLE COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(${CMAKE_PROJECT_NAME}               PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Source        PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Serialization PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Core          PRIVATE ${INCLUDE_DIRECTORIES})

target_precompile_headers(${CMAKE_PROJECT_NAME}.Source        PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Managers      REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics      REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Graphics.Core REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.ECS           REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.ECSSystems    REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Components    REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Serialization REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Core          REUSE_FROM ${CMAKE_PROJECT_NAME}.Source)

target_compile_definitions(${CMAKE_PROJECT_NAME}               PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Source        PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Serialization PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Core          PRIVATE ${DEFINES})

target_link_directories(${CMAKE_PROJECT_NAME}               PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Source        PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Managers      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.ECS           PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Components    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Serialization PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Core          PRIVATE ${LINK_DIRECTORIES})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS true)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_PROJECT_NAME}.Source
    GLAD
    IMGUI
    STB_IMAGE
    glfw3
    glm
    fmt
    assimp
    zlibstatic
    TracyClient
    opengl32
    gdi32
    user32
    kernel32
    shell32
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")  # for some reason Tracy raises an error in Debug mode
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Tracy::TracyClient)
endif()