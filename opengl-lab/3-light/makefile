all:
	g++ -c main.cpp -o obj/main.o -I$(LIBS)/freeglut-3.0.0/include -std=c++17
	g++ obj/main.o -o run.exe -lOpenGL32 -lglu32 -lfreeglut -L$(LIBS)/freeglut-3.0.0/lib

run: all
	./run.exe