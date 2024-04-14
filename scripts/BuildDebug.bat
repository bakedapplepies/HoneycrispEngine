cd ..
cmake -S . -B build/Debug -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Debug || exit
cd build/Debug
ninja || exit