REM Build script for Game
@ECHO OFF
SetLocal EnableDelayedExpansion

cd build

ECHO "Running CMake for game..."
cmake -G Ninja .. -DCMAKE_C_COMPILER=clang -DBUILD_GAME=ON

ECHO "Building game exe from CMake..."
ninja