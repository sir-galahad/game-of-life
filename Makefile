

life : lifelib.o display.o
	gcc -g lifelib.o display.o -lncurses -o life

display.o : display.c
	gcc display.c -c -g -o display.o

lifelib.o : lifelib.c
	gcc lifelib.c -c -g -o lifelib.o

clean :
	rm life
	rm *.o
