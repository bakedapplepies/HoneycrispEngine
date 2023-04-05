g++ -g ./bin/intermediates/*.obj -o ./bin/main.exe -L./lib -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lshell32 || exit

.\bin\main.exe