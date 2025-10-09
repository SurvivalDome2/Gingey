REM Build script for Ginger
@ECHO OFF
SetLocal EnableDelayedExpansion

cd build

ECHO "Running CMake for engine..."
cmake -G Ninja .. -DCMAKE_C_COMPILER=clang -DBUILD_ENGINE=ON

ECHO "Building engine lib from CMake..."
ninja