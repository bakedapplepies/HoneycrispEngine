:: Compile -> .obj
g++ -g -c -std=c++17 -I./include ./src/Debug.cpp -o ./bin/intermediates/Debug.obj || exit

g++ -g -c -std=c++17 -I./include ./src/glad.c -o ./bin/intermediates/glad.obj || exit

g++ -g -c -std=c++17 -I./include ./src/main.cpp -o ./bin/intermediates/main.obj || exit

g++ -g -c -std=c++17 -I./include ./src/VertexArray.cpp -o ./bin/intermediates/VertexArray.obj || exit

g++ -g -c -std=c++17 -I./include ./src/VertexBuffer.cpp -o ./bin/intermediates/VertexBuffer.obj || exit

g++ -g -c -std=c++17 -I./include ./src/ElementBuffer.cpp -o ./bin/intermediates/ElementBuffer.obj || exit

g++ -g -c -std=c++17 -I./include ./src/Shader.cpp -o ./bin/intermediates/Shader.obj || exit

g++ -g -c -std=c++17 -I./include ./src/Texture.cpp -o ./bin/intermediates/Texture.obj || exit

g++ -g -c -std=c++17 -I./include ./src/stb_image.cpp -o ./bin/intermediates/stb_image.obj || exit

:: g++ -g -c -std=c++17 -I./include ./src/Camera.cpp -o ./bin/intermediates/Camera.obj || exit

g++ -g -c -std=c++17 -I./include ./src/Window.cpp -o ./bin/intermediates/Window.obj || exit

:: Linking
g++ -g ./bin/intermediates/*.obj -o ./bin/main.exe -L./lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 || exit
