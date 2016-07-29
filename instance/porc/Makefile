proc:porc.o
	gcc -o porc porc.o
proc.o:porc.s
	gcc -c porc.s -o porc.o
porc.s:porc.i
	gcc -S porc.i -o porc.s
porc.i:porc.c
	gcc -E porc.c -o porc.i
.PHONY:clean
clean:
	rm -rf porc.i porc.s porc.o porc
