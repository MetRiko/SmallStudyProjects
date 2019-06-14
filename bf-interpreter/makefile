all:
	g++ -o run.exe main.c

run: all
	./run

disasm:
	g++ -S -Wa,-adhln main.c -o main.s

asm:
	g++ main.s -o runasm.exe

runasm: asm
	./runasm

# %rdi %rsi %rdx %rcx %r8 %r9