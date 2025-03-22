cd ..
mkdir build
cd build
mkdir Debug
cd ../..
cmake -S . -B build/Release -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_STANDARD=20 || exit
cd build/Release
ninja || exit