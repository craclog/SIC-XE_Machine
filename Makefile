sicxe.out : sicxe.o
	gcc -o sicxe.out sicxe.o

sicxe.o : sicxe.c
	gcc -Wall -c -o sicxe.o sicxe.c

clean:
	rm *.o sicxe.out
