set(RENDERER_SOURCES
    Renderer.cpp
    PostprocessQueue.cpp
    meshes/Quad.cpp
    meshes/Model.cpp
)

add_library(${PROJECT_NAME}.Renderer
    ${RENDERER_SOURCES}
)