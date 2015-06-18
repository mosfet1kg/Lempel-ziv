OBJECTS = main.o list.o

test : $(OBJECTS)
	gcc -o test $(OBJECTS) -lm

main.o : main.c main.h
	gcc -c main.c main.h -lm
list.o : lib/list.h lib/list.c lib/debug.h
	gcc -c lib/list.c -lm

clean :
	rm $(OBJECTS) test