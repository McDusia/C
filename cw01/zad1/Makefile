CFLAGS = -Wall -pedantic -std=c11


libstatic1: Zadanie1.c
	gcc $(CFLAGS) -c -Wall Zadanie1.c
	ar -crs libstatic1.a Zadanie1.o

libstatic2: Zadanie2.c
	gcc $(CFLAGS) -c -Wall Zadanie2.c
	ar -crs libstatic2.a Zadanie2.o

libshared1: Zadanie1.c
	gcc $(CFLAGS) -Wall -fPIC -c Zadanie1.c
	gcc -shared -fPIC -o libshared1.so Zadanie1.o

libshared2: Zadanie2.c
	gcc $(CFLAGS) -Wall -fPIC -c Zadanie2.c
	gcc -shared -fPIC -O2 -g -o libshared2.so Zadanie2.o
	
	
cleanSt: 
	 rm libstatic1.a
	 rm Zadanie1.o
	rm libstatic2.a
	 rm Zadanie2.o

cleanSh: 
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o

cleanAll:
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o
	rm libstatic1.a
	rm libstatic2.a
