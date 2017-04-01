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
	
testStat:
	make libstatic1
	make libstatic2
	gcc $(CFLAGS) -O2 -g timeTest.c -L. -lstatic1 -lstatic2 -o testStat

testShar:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O2 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testShar

testDyn:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O2 -g timeTest_dyn.c -L. -o testDyn -ldl


test:
	make testStat
	make testShar
	make testDyn
	
	echo "" >> results.txt
	echo "static timeTest" >> results.txt
	./testStat >> results.txt
	echo "shared timeTest" >> results.txt
	./testShar >> results.txt
	echo "dynamic timeTest" >> results.txt
	./testDyn >> results.txt
	
cleanSt: 
	 rm libstatic1.a
	 rm Zadanie1.o
	rm libstatic2.a
	 rm Zadanie2.o
	rm testStat

cleanSh: 
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o
	rm testShar

cleanD:
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o
	rm testDyn

cleanAll:
	 rm Zadanie1.o	
	 rm Zadanie2.o	
	rm libshared1.so
	rm libshared2.so
	rm libstatic1.a
	rm libstatic2.a
	rm testStat
	rm testShar
	rm testDyn
