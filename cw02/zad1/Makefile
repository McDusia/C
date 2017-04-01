CFLAGS = -Wall -pedantic -std=c11

comparator:
	gcc $(CFLAGS) -O2 comparator.c -o comparator


test: comparator
	./comparator sys generate file.txt 100 4 >> wyniki.txt
	cp file.txt copy.txt
	echo -e "" >> wyniki.txt
	echo -e "System functions - shuffle operation \n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator sys shuffle file.txt 100 4 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "System functions - bubble operation \n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator sys bubble file.txt 100 4 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - bubble sort operation\n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator lib bubble file.txt 100 4 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - shuffle sort operation\n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator lib shuffle file.txt 100 4 >> wyniki.txt
	cp copy.txt file.txt
	./comparator sys generate file.txt 100 512 >> wyniki.txt
	cp file.txt copy.txt		
	echo -e "System functions - shuffle operation \n Record size: 512\n Records quantity: 100" >> wyniki.txt
	./comparator sys shuffle file.txt 100 512 >> wyniki.txt
	cp copy.txt file.txt	
	echo -e "System functions - bubble operation\n Record size: 512\n Records quantity: 100" >> wyniki.txt
	./comparator sys bubble file.txt 100 512 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - bubble sort operation\n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator lib bubble file.txt 100 512 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - shuffle sort operation\n Record size: 4\n Records quantity: 100" >> wyniki.txt
	./comparator lib shuffle file.txt 100 512 >> wyniki.txt
	cp copy.txt file.txt
	./comparator sys generate file.txt 100 4096 >> wyniki.txt
	cp file.txt copy.txt
	echo -e "System functions - shuffle operation \n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator sys shuffle file.txt 100 4096 >> wyniki.txt
	cp copy.txt file.txt	
	echo -e "System functions - bubble operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator sys bubble file.txt 100 4096 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - bubble sort operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator lib bubble file.txt 100 4096 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - shuffle sort operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator lib shuffle file.txt 100 4096 >> wyniki.txt
	cp copy.txt file.txt
	
	./comparator sys generate file.txt 100 8192 >> wyniki.txt
	cp file.txt copy.txt
	echo -e "System functions - shuffle operation \n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator sys shuffle file.txt 100 8192 >> wyniki.txt
	cp copy.txt file.txt	
	echo -e "System functions - bubble operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator sys bubble file.txt 100 8192 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - bubble sort operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator lib bubble file.txt 100 8192 >> wyniki.txt
	cp copy.txt file.txt
	echo -e "Library functions - shuffle sort operation\n Record size: 4096\n Records quantity: 100" >> wyniki.txt
	./comparator lib shuffle file.txt 100 8192 >> wyniki.txt
	cp copy.txt file.txt

c:
	rm comparator
