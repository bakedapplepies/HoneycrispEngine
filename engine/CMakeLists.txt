add_subdirectory(vendor)
add_subdirectory(renderer)
add_subdirectory(editor)
add_subdirectory(util)

add_library(IHoneycrispEngine INTERFACE)

target_include_directories(IHoneycrispEngine INTERFACE
    ${PROJECT_SOURCE_DIR}/engine
)

target_link_libraries(IHoneycrispEngine INTERFACE
    GLFW_LIBRARY
    TRACY_LIBRARY
    ASSIMP_LIBRARY
    IMGUI_LIBRARY
    ENVYENGINE_LIBRARY
)

target_compile_definitions(IHoneycrispEngine INTERFACE
    HNCRSP_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

# Build-type configurations
target_compile_options(IHoneycrispEngine INTERFACE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3 -DNDEBUG
    >
)

target_link_options(IHoneycrispEngine INTERFACE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3
    >
)

target_link_libraries(${PROJECT_NAME}.Util PRIVATE IHoneycrispEngine)
target_link_libraries(${PROJECT_NAME}.Renderer PRIVATE IHoneycrispEngine ${PROJECT_NAME}.Util)
target_link_libraries(${PROJECT_NAME}.Editor PRIVATE IHoneycrispEngine ${PROJECT_NAME}.Renderer)

add_library(HoneycrispEngine INTERFACE)
target_link_libraries(HoneycrispEngine INTERFACE
    IHoneycrispEngine
    ${PROJECT_NAME}.Editor
)