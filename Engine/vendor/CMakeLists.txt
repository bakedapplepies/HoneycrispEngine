include(FetchContent)

if (UNIX)
    set(LIB lib64)
elseif (WIN32)
    set(LIB lib)
endif ()

# ======================================================================
FetchContent_Declare(glfw_fetch
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG dbadda26835ec5089ef922e6c290bcf58cf12056
)
FetchContent_MakeAvailable(glfw_fetch)
FetchContent_GetProperties(glfw_fetch
    SOURCE_DIR glfw_SOURCE_DIR
    BINARY_DIR glfw_BINARY_DIR
)
add_library(GLFW_LIBRARY INTERFACE IMPORTED GLOBAL)
target_include_directories(GLFW_LIBRARY INTERFACE
    ${glfw_SOURCE_DIR}/include
)
target_link_libraries(GLFW_LIBRARY INTERFACE
    ${glfw_BINARY_DIR}/src/libglfw3.a
)

# ======================================================================
set(ASSIMP_NO_EXPORT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(assimp_fetch
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG e0b52347c6e52de2827ec957a9ebf00ce3c54f79
)
FetchContent_MakeAvailable(assimp_fetch)
FetchContent_GetProperties(assimp_fetch
    SOURCE_DIR assimp_SOURCE_DIR
    BINARY_DIR assimp_BINARY_DIR
)
add_library(ASSIMP_LIBRARY INTERFACE IMPORTED GLOBAL)
target_include_directories(ASSIMP_LIBRARY INTERFACE
    ${assimp_SOURCE_DIR}/include
)
target_link_libraries(ASSIMP_LIBRARY INTERFACE
    assimp
)

# ======================================================================
if (${USE_LOCAL_ENVY_ENGINE})
    FetchContent_Declare(envyengine_fetch
        SOURCE_DIR ${LOCAL_ENVY_ENGINE_ROOT}
    )
else()
    FetchContent_Declare(envyengine_fetch
        GIT_REPOSITORY https://github.com/bakedapplepies/EnvyEngine.git
        GIT_TAG a6222ed31f0c7d389b14219d17e229d1a7156139
    )
endif()

FetchContent_MakeAvailable(envyengine_fetch)
add_library(ENVYENGINE_LIBRARY ALIAS EnvyEngine)