CC=g++
DEV=-Wall -g -std=c++11
OPT=-O3 -std=c++11

main:demos.c
	gcc -o main demos.c -std=c++11 -Wall

clean:
	rm -f *.o
	rm -r demos