LDLIBS = -pthread -lrt

b: barber.c
	gcc -Wall $(LDLIBS) -pedantic barber.c -o b

m: master.c
	gcc $(LDLIBS) -Wall -pedantic master.c -o m

a: barber.c master.c
	gcc $(LDLIBS) -Wall -pedantic barber.c -o b
	gcc $(LDLIBS) -Wall -pedantic master.c -o m

clean: 
	 rm m
	 rm b

