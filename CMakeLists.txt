cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 20)

# Compiler Options
# add_compile_options(-v -g)
# add_link_options(-v -g)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fsanitize=address -ffile-prefix-map=${CMAKE_SOURCE_DIR}=. -O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -fsanitize=address")

project(LearnOpenGL CXX C)

# -L
link_directories(./lib)

# Sub-Dirs
# add_subdirectory(src)

# Libraries
add_library(GLAD STATIC
    ./include/glad/glad.c
)
target_include_directories(GLAD PUBLIC include)
set_target_properties(GLAD PROPERTIES LINKER_LANGUAGE C)

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

add_library(FMT STATIC
    ./lib/format.cc
)
target_include_directories(FMT PUBLIC include include/fmt)
set_target_properties(FMT PROPERTIES LINKER_LANGUAGE CXX)

set(CORE
	./src/core/VertexBuffer.cpp
	./src/core/VertexArray.cpp
	./src/core/Texture.cpp
	./src/core/Shader.cpp
	./src/core/ElementBuffer.cpp
)

set(SCENES
	./src/scenes/DefaultSceneTwo.cpp
	./src/scenes/DefaultScene.cpp
)

set(APPLICATION
	./src/Window.cpp
	./src/SceneManager.cpp
	./src/Scene.cpp
	./src/Model.cpp
	./src/Mesh.cpp
	./src/main.cpp
	./src/Light.cpp
    ./src/Cubemap.cpp
	./src/Debug.cpp
	./src/Cube.cpp
	./src/Camera.cpp
	./src/Application.cpp
)

# Executable
add_executable(${CMAKE_PROJECT_NAME}
    ${APPLICATION}
    ${CORE}
    ${SCENES}
)

# -I
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC include include/assimp)
message("\n-- Project root: ${PROJECT_SOURCE_DIR}")

# What to link
set(LIBS
    GLAD
    STB_IMAGE
    IMGUI
    FMT
    libglfw3.a
    libassimp.dll.a
    opengl32
    gdi32
    user32
    kernel32
    shell32
)

# Linking
target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${LIBS}
)

target_precompile_headers(${CMAKE_PROJECT_NAME}
    PRIVATE
        ./src/pch/pch.h
)

# add dll(s)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/lib/libassimp-5.dll"
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)