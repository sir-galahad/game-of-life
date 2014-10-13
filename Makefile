

life : lifelib.o display.o
	gcc lifelib.o display.o -lncurses -o life

display.o : display.c
	gcc display.c -c -o display.o

lifelib.o : lifelib.c
	gcc lifelib.c -c -o lifelib.o

clean :
	rm life
	rm *.o
