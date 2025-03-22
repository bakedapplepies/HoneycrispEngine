cd ..
mkdir build
cd build
mkdir Debug
cd ../..
cmake -S . -B build/Debug -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_CXX_STANDARD=20 || exit
cd build/Debug
ninja || exit