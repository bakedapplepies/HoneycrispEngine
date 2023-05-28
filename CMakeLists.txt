cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

# Compiler Options
# add_compile_options(-v -g)
# add_link_options(-v -g)
message("Hi: ${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -fsanitize=address")

project(LearnOpenGL CXX)

# -L
link_directories(./lib)

# Sub-Dirs
# add_subdirectory(src)

# Libraries
add_library(GLAD STATIC
    ./include/glad/glad.cpp
)
target_include_directories(GLAD PUBLIC include)
set_target_properties(GLAD PROPERTIES LINKER_LANGUAGE CXX)

add_library(STB_IMAGE STATIC
    ./include/stb/stb_image.cpp
)
target_include_directories(STB_IMAGE PUBLIC include include/stb)
set_target_properties(STB_IMAGE PROPERTIES LINKER_LANGUAGE CXX)

add_library(IMGUI STATIC
    ./include/imgui/imgui.cpp
    ./include/imgui/imgui_draw.cpp
    ./include/imgui/imgui_tables.cpp
    ./include/imgui/imgui_widgets.cpp
    ./include/imgui/imgui_impl_glfw.cpp
    ./include/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(IMGUI PUBLIC include include/imgui)
set_target_properties(IMGUI PROPERTIES LINKER_LANGUAGE CXX)

# Executable
add_executable(${CMAKE_PROJECT_NAME}
    # Sources
    ./include/glad/glad.cpp
    ./src/Debug.cpp
    ./src/main.cpp
    ./src/pch/pch.cpp
    ./src/Camera.cpp
    ./src/Cube.cpp
    ./src/Light.cpp
    ./src/VertexBuffer.cpp
    ./src/ElementBuffer.cpp
    ./src/VertexArray.cpp
    ./src/Mesh.cpp
    ./src/Shader.cpp
    ./src/Texture.cpp
    ./src/Window.cpp

    # Headers
    ./src/pch/pch.h
    ./src/Callbacks.h
    ./src/constants.h
    ./src/Object.h
    ./src/Camera.h
    ./src/Cube.h
    ./src/Light.h
    ./src/Debug.h
    ./src/VertexBuffer.h
    ./src/ElementBuffer.h
    ./src/VertexArray.h
    ./src/Mesh.h
    ./src/Shader.h
    ./src/Texture.h
    ./src/Window.h
)

# -I
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ./include)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ./src)

# What to link
set(LIBS
    # GLAD
    STB_IMAGE
    IMGUI
    libglfw3.a
    opengl32
    gdi32
    user32
    kernel32
    shell32
)

# Linking
target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${LIBS}
)