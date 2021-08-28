all: main.o reg.o
	gcc -m32  main.o reg.o -pthread  -o out
main.o:
	gcc -m32  -c -o main.o main.c
reg.o:
	nasm enter_leave.asm -o reg.o -f elf
clean:
	rm *.o