CFLAGS= -Wall -fpic -coverage -lm -std=c99

testme: testme.c
	gcc -o testme -g testme.c $(CFLAGS)
	./testme

all: testme

clean:
	rm -f *.o *.gcov *.gcda *.gcno testme testme.exe
