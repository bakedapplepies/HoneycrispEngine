cmake_minimum_required(VERSION 3.26.0)
project(Honeycrisp CXX C)

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
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Engine
    ${CMAKE_SOURCE_DIR}/Engine/include
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
    ${CMAKE_SOURCE_DIR}/Engine/include/api/pch/hncrsp_pch.h
)
set(DEFINES
    HNCRSP_ENGINE
)

add_library(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Engine/src/main.cpp)

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
add_subdirectory(vendor)
add_subdirectory(Engine/src)
add_subdirectory(TestClient/src)

# TODO: some libraries are Windows-specific
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_PROJECT_NAME}.Engine.Source
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
)

target_compile_options(${CMAKE_PROJECT_NAME}.Engine.Source PRIVATE
    ${COMPILE_OPTIONS}    
)

# Set macro as git commit id
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD OUTPUT_VARIABLE COMMIT_ID OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(${CMAKE_PROJECT_NAME}                      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Source        PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Managers      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Graphics      PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Graphics.Core PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.ECS           PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.ECSSystems    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Components    PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Serialization PRIVATE ${INCLUDE_DIRECTORIES})
target_include_directories(${CMAKE_PROJECT_NAME}.Engine.Core          PRIVATE ${INCLUDE_DIRECTORIES})

target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Source        PRIVATE ${PCH})
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Managers      REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Graphics      REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Graphics.Core REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.ECS           REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.ECSSystems    REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Components    REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Serialization REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)
target_precompile_headers(${CMAKE_PROJECT_NAME}.Engine.Core          REUSE_FROM ${CMAKE_PROJECT_NAME}.Engine.Source)

target_compile_definitions(${CMAKE_PROJECT_NAME}                      PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Source        PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Managers      PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Graphics      PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Graphics.Core PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.ECS           PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.ECSSystems    PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Components    PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Serialization PRIVATE ${DEFINES})
target_compile_definitions(${CMAKE_PROJECT_NAME}.Engine.Core          PRIVATE ${DEFINES})

target_link_directories(${CMAKE_PROJECT_NAME}                      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Source        PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Managers      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Graphics      PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Graphics.Core PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.ECS           PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.ECSSystems    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Components    PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Serialization PRIVATE ${LINK_DIRECTORIES})
target_link_directories(${CMAKE_PROJECT_NAME}.Engine.Core          PRIVATE ${LINK_DIRECTORIES})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VERIFY_INTERFACE_HEADER_SETS true)
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")  # for some reason Tracy raises an error in Debug mode
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Tracy::TracyClient)
endif()

# Build-type configurations
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-O0 -fsanitize=address -g)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address -g)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O3)
    add_link_options(-O3)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    message("MinSizeRel config not supported yet!")
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    message("RelWithDebInfo config not supported yet!")

endif()
