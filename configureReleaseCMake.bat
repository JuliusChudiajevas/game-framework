cmake -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE=Release -S . -B ./release/ -G "MinGW Makefiles"