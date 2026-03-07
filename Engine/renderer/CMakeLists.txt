set(RENDERER_SOURCES
    AppWindow.cpp
    Renderer.cpp
    meshes/Quad.cpp
)

add_library(${PROJECT_NAME}.Renderer
    ${RENDERER_SOURCES}
)