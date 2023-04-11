@REM glad
g++ -g -c -std=c++17 -I./include ./include/glad/glad.c -o ./bin/intermediates/glad.obj || exit

@REM stb_image
g++ -g -c -std=c++17 -I./include ./include/stb/stb_image.cpp -o ./bin/intermediates/stb_image.obj || exit

@REM imgui
g++ -g -c -std=c++17 -I./include ./include/imgui/imgui.cpp -o ./bin/intermediates/imgui.obj || exit

g++ -g -c -std=c++17 -I./include ./include/imgui/imgui_draw.cpp -o ./bin/intermediates/imgui_draw.obj || exit

g++ -g -c -std=c++17 -I./include ./include/imgui/imgui_tables.cpp -o ./bin/intermediates/imgui_tables.obj || exit

g++ -g -c -std=c++17 -I./include ./include/imgui/imgui_widgets.cpp -o ./bin/intermediates/imgui_widgets.obj || exit

g++ -g -c -std=c++17 -I./include ./include/imgui/imgui_impl_glfw.cpp -o ./bin/intermediates/imgui_impl_glfw.obj || exit

g++ -g -c -std=c++17 -I./include ./include/imgui/imgui_impl_opengl3.cpp -o ./bin/intermediates/imgui_impl_opengl3.obj || exit

@REM precompied headers
g++ -I./include ./src/pch/pch.h
