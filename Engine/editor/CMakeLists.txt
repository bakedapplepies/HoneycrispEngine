set(EDITOR_SOURCES
    GLFWCallbacks.cpp
    Camera.cpp
    Material.cpp
    Transform.cpp
)

add_library(${PROJECT_NAME}.Editor
    ${EDITOR_SOURCES}
)