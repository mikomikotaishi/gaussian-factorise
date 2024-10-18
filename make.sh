#!/bin/bash

# Generate the build directory
mkdir -p build

# Compile the headers
echo "Compiling headers."
if [ ! -d "gcm.cache" ]; then
    g++ -std=c++20 -fmodules-ts -c -x c++-system-header algorithm array cassert cctype climits cmath iostream random regex stdexcept string vector
else
    echo "gcm.cache directory found. Skipping header compilation."
fi

# Compile the modules
echo "Compiling modules."
g++ -std=c++20 -fmodules-ts -c -Wall -g src/utility/inputparser.cppm -o build/inputparser.o
g++ -std=c++20 -fmodules-ts -c -Wall -g src/gaussian_integer/gaussian_integer.cppm -o build/gaussian_integer.o
g++ -std=c++20 -fmodules-ts -c -Wall -g src/utility/inputparser.cpp -o build/inputparser_impl.o
g++ -std=c++20 -fmodules-ts -c -Wall -g src/gaussian_integer/gaussian_integer.cpp -o build/gaussian_integer_impl.o
g++ -std=c++20 -fmodules-ts -c -Wall -g src/main.cpp -o build/main.o

# Link object files
echo "Linking object files."
g++ -o gaussian-factorise build/main.o build/gaussian_integer.o build/gaussian_integer_impl.o build/inputparser.o build/inputparser_impl.o
