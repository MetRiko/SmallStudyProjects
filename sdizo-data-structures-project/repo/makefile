all:
	g++ -c main.cpp -fdiagnostics-color=always -static -static-libstdc++ -static-libgcc
	g++ -o run.exe main.o -static -static-libstdc++ -static-libgcc

run: all
	./run