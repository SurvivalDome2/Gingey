REM Build script for CMake
@ECHO OFF
SetLocal EnableDelayedExpansion

cd build

ECHO "Running CMake for engine..."
cmake -G Ninja .. -DCMAKE_C_COMPILER=clang -DBUILD_ENGINE=ON

ECHO "Running CMake for game..."
cmake -G Ninja .. -DCMAKE_C_COMPILER=clang -DBUILD_GAME=ON

ECHO "Building engine dll and game exe from CMake..."
ninja