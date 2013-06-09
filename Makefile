CFLAGS=-c

all: Othello

Othello: othello.o
	g++ Wang_Zhou.cpp -o Othello
clean:
	rm -rf othello.o
