:: Compile -> .obj
g++ -g -c -std=c++17 -I./include ./src/glad.c -o ./bin/intermediates/glad.obj || exit
g++ -g -c -std=c++17 -I./include ./src/Shader.cpp -o ./bin/intermediates/Shader.obj || exit
g++ -g -c -std=c++17 -I./include ./src/stb_image.cpp -o ./bin/intermediates/stb_image.obj || exit
g++ -g -c -std=c++17 -I./include ./src/main.cpp -o ./bin/intermediates/main.obj || exit

:: Linking
g++ -g ./bin/intermediates/*.obj -o ./bin/main.exe -L./lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 || exit
