clang++ make_zim.cpp -o make_zim -Ivendor $(pkg-config --cflags --libs libzim) -std=c++17

