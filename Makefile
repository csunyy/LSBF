CC = gcc
all: lsbf exact_query

lsbf: lsbf.c bloom.h bloom.o hash.o
	$(CC) lsbf.c bloom.o hash.o -o $@  -lm

exact_query: exact_query.c bloom.h bloom.o
	$(CC) exact_query.c bloom.o hash.o -o $@ -lm

bloom.o: bloom.c bloom.h hash.h 
	$(CC) -c bloom.c   

hash.o: hash.c hash.h
	$(CC) -c hash.c
clean:
	rm *.o lsbf exact_query
