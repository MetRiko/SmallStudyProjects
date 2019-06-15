all:
	g++ -c main.cpp -o main.o -fconcepts
	g++ -o run.exe main.o

lnx:
	g++ -c main.cpp -o main.o -fconcepts -DNCURSES
	g++ -o run main.o

lnxrun: lnx
	./run

run: all
	./run.exe