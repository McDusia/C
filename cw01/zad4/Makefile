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

testStat0:
	make libstatic1
	make libstatic2
	gcc $(CFLAGS) -O0 -g timeTest.c -L. -lstatic1 -lstatic2 -o testStat0

testStat1:
	make libstatic1
	make libstatic2
	gcc $(CFLAGS) -O1 -g timeTest.c -L. -lstatic1 -lstatic2 -o testStat1

testStat2:
	make libstatic1
	make libstatic2
	gcc $(CFLAGS) -O2 -g timeTest.c -L. -lstatic1 -lstatic2 -o testStat2

testStats:
	make libstatic1
	make libstatic2
	gcc $(CFLAGS) -Os -g timeTest.c -L. -lstatic1 -lstatic2 -o testStats



testShar0:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O0 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testShar0

testShar1:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O1 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testShar1

testShar2:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O2 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testShar2

testShars:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -Os -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testShars


testDyn0:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O0 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testDyn0 -lshared1 -lshared2 -ldl

testDyn1:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O1 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testDyn1 -lshared1 -lshared2 -ldl

testDyn2:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -O2 -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testDyn2 -lshared1 -lshared2 -ldl

testDyns:
	make libshared1
	make libshared2
	gcc $(CFLAGS) -Os -g timeTest.c -L. -Wl,-rpath=. -lshared1 -lshared2 -o testDyns -lshared1 -lshared2 -ldl



test:
	make testStat0
	make testStat1
	make testStat2
	make testStats	
	make testShar0
	make testShar1
	make testShar2
	make testShars
	make testDyn0
	make testDyn1
	make testDyn2
	make testDyns
	
	
	echo "" >> results.txt
	echo "static timeTest optimisation level: 0" >> results.txt
	./testStat0 >> results.txt
	echo "static timeTest optimisation level: 1" >> results.txt
	./testStat1 >> results.txt
	echo "static timeTest optimisation level: 2" >> results.txt
	./testStat2 >> results.txt
	echo "static timeTest optimisation level: s" >> results.txt
	./testStats >> results.txt
	echo "shared timeTest optimisation level: 0" >> results.txt
	./testShar0 >> results.txt
	echo "shared timeTest optimisation level: 1" >> results.txt
	./testShar1 >> results.txt
	echo "shared timeTest optimisation level: 2" >> results.txt
	./testShar2 >> results.txt
	echo "shared timeTest optimisation level: s" >> results.txt
	./testShars >> results.txt
	echo "dynamic timeTest optimisation level: 0" >> results.txt
	./testDyn0 >> results.txt
	echo "dynamic timeTest optimisation level: 1" >> results.txt
	./testDyn1 >> results.txt
	echo "dynamic timeTest optimisation level: 2" >> results.txt
	./testDyn2 >> results.txt
	echo "dynamic timeTest optimisation level: s" >> results.txt
	./testDyns >> results.txt	
	
cleanSt: 
	rm libstatic1.a
	rm libstatic2.a
	 rm Zadanie1.o
	rm Zadanie2.o
	rm testStat0
	rm testStat1
	rm testStat2
	rm testStats
cleanSh: 
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o
	rm testShar0
	rm testShar1
	rm testShar2
	rm testShars
cleanD:
	rm libshared1.so
	 rm Zadanie1.o
	rm libshared2.so
	 rm Zadanie2.o
	rm testDyn0
	rm testDyn1
	rm testDyn2
	rm testDyns
cleanAll:
	rm libstatic1.a
	rm libstatic2.a
	rm libshared1.so
	rm libshared2.so
	rm Zadanie1.o
	rm Zadanie2.o
	rm testStat0
	rm testStat1
	rm testStat2
	rm testStats
	rm testShar0
	rm testShar1
	rm testShar2
	rm testShars
	rm testDyn0
	rm testDyn1
	rm testDyn2
	rm testDyns
