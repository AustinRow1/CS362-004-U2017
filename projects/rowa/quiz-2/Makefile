
testme.o: testme.c
	gcc -c testme.c

testme: testme.o
	gcc testme.o -o testme
	./testme

all: testme

clean:
	rm -f ./testme ./testme.o
