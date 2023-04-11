:: Compile -> .obj
g++ -g -c -std=c++17 -I./include -I./src/pch ./src/pch/pch.cpp -o ./bin/intermediates/pch.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Debug.cpp -o ./bin/intermediates/Debug.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/main.cpp -o ./bin/intermediates/main.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/VertexArray.cpp -o ./bin/intermediates/VertexArray.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/VertexBuffer.cpp -o ./bin/intermediates/VertexBuffer.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/ElementBuffer.cpp -o ./bin/intermediates/ElementBuffer.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Shader.cpp -o ./bin/intermediates/Shader.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Texture.cpp -o ./bin/intermediates/Texture.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Cube.cpp -o ./bin/intermediates/Cube.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Light.cpp -o ./bin/intermediates/Light.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Camera.cpp -o ./bin/intermediates/Camera.obj || exit

g++ -g -c -std=c++17 -I./include -I./src/pch ./src/Window.cpp -o ./bin/intermediates/Window.obj || exit

:: Linking
g++ -g ./bin/intermediates/*.obj -o ./bin/main.exe -L./lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 || exit
