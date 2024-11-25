#!/bin/bash
original_dir=$(pwd)
program_name="./gaussian-factorise"

clean=false
run=false
run_args=()

while getopts "cr:" opt; do
    case $opt in
        c) clean=true ;;
        r) run=true ; run_args+=("$OPTARG") ;;
        *) echo "Usage: $0 [-c] [-r <args>]"; exit 1 ;;
    esac
done

if [[ $clean == false && $run == false ]]; then
    echo "No options provided. Defaulting to build."
elif $clean; then
    echo "Cleaning build directory..."
    rm -rf build
    echo "Done cleaning"
    exit 0
fi

mkdir -p build
cd build || exit 1
if command -v clang++ &> /dev/null; then
    echo "Compiling using Clang:"
    cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++ -lc++abi" ..
else
    echo "Compiling using GCC:"
    cmake -G Ninja -DCMAKE_CXX_COMPILER=g++ ..
fi
echo "Building project..."
cmake --build .

if $run; then
    if [[ -x $program_name ]]; then
        echo "Running program with arguments: ${run_args[*]}"
        $program_name "${run_args[@]}"
    else
        echo "Error: Program '$program_name' not found or not executable."
        exit 1
    fi
fi

cd "$original_dir" || exit 1