add_subdirectory(vendor)
add_subdirectory(renderer)
add_subdirectory(editor)
add_subdirectory(util)

add_library(HoneycrispEngineI INTERFACE)

target_include_directories(HoneycrispEngineI INTERFACE
    ${PROJECT_SOURCE_DIR}/engine
)

target_link_libraries(HoneycrispEngineI INTERFACE
    GLFW_LIBRARY
    ASSIMP_LIBRARY
    ENVYENGINE_LIBRARY
)

target_compile_definitions(HoneycrispEngineI INTERFACE
    HNCRSP_ROOT_DIR="${PROJECT_SOURCE_DIR}"
)

# Build-type configurations
target_compile_options(HoneycrispEngineI INTERFACE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3 -DNDEBUG
    >
)

target_link_options(HoneycrispEngineI INTERFACE
    $<IF:$<CONFIG:Debug>,
        -O0 -g -fsanitize=address,
        -O3
    >
)

target_link_libraries(${PROJECT_NAME}.Renderer PRIVATE HoneycrispEngineI)
target_link_libraries(${PROJECT_NAME}.Editor PRIVATE HoneycrispEngineI)
target_link_libraries(${PROJECT_NAME}.Util PRIVATE HoneycrispEngineI)

add_library(HoneycrispEngine INTERFACE)
target_link_libraries(HoneycrispEngine INTERFACE ${PROJECT_NAME}.Renderer)
target_link_libraries(HoneycrispEngine INTERFACE ${PROJECT_NAME}.Editor)
target_link_libraries(HoneycrispEngine INTERFACE ${PROJECT_NAME}.Util)
target_link_libraries(HoneycrispEngine INTERFACE HoneycrispEngineI)