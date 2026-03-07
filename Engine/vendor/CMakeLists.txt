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