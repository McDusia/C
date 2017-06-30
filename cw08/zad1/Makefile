LDLIBS = -lpthread

zad1a: zad1a.c
	gcc -Wall -g -pedantic zad1a.c $(LDLIBS) -o zad1a
zad1b: zad1b.c
	gcc -Wall -g -pedantic zad1b.c $(LDLIBS) -o zad1b
zad1c: zad1c.c
	gcc -Wall -g -pedantic zad1c.c $(LDLIBS) -o zad1c
zad1: zad1a.c zad1b.c zad1c.c
	gcc -Wall -g -pedantic zad1a.c $(LDLIBS) -o zad1a
	gcc -Wall -g -pedantic zad1b.c $(LDLIBS) -o zad1b
	gcc -Wall -g -pedantic zad1c.c $(LDLIBS) -o zad1c
clean: 
	rm zad1a
	rm zad1b
	rm zad1c

