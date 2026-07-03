set(EDITOR_SOURCES
    AppWindow.cpp
    GLFWCallbacks.cpp
)

add_library(${PROJECT_NAME}.Editor
    ${EDITOR_SOURCES}
)