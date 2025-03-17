#!/bin/sh

version=$(grep "^[[:space:]]*VERSION" CMakeLists.txt|xargs echo|cut -d " " -f 2)
filename='smokestack'
rm -f "${filename}-${version}.tar.xz"
cd src || exit
emcc -lembind -o smokestack.js smokestack.cpp -O2 -std=c++23
mkdir -p "${filename}-${version}/src"
mkdir -p "${filename}-${version}/html"
cp ../LICENSE ../AUTHORS "${filename}-${version}"
cp smokestack.h smokestack.cpp "${filename}-${version}/src"
cp ./*.wasm ./*.js ./*.html ./*.svg "${filename}-${version}/html"
tar cJf "${filename}-${version}.tar.xz" "${filename}-${version}"
rm -rf ./*.wasm ./*.js "${filename}-${version}"
mv "${filename}-${version}.tar.xz" ..
cd .. || exit
tar tJf "${filename}-${version}.tar.xz"
