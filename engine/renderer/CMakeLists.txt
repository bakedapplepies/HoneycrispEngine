set(RENDERER_SOURCES
    ForwardRenderer.cpp
    DeferredRenderer.cpp
    PostprocessQueue.cpp
    Camera.cpp
    Material.cpp
    Transform.cpp
    meshes/Quad.cpp
    meshes/Model.cpp
)

add_library(${PROJECT_NAME}.Renderer
    ${RENDERER_SOURCES}
)