all:
	g++ -g -c main.cpp -fdiagnostics-color -std=c++17 -static -static-libstdc++ -static-libgcc
	g++ -g -o run.exe main.o -std=c++17 -static -static-libstdc++ -static-libgcc

run: all
	./run.exe