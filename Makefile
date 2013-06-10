CFLAGS=-c

all: Othello

#Othello: Wang_Zhou.o
#	g++ Wang_Zhou.o -o Othello
Othello: MatchMaking.o
	g++ MatchMaking.o -o Othello

#Wang_Zhou.o: Wang_Zhou.cpp
#	g++ -c Wang_Zhou.cpp
MatchMaking.o: MatchMaking.cpp
	g++ -c MatchMaking.cpp

clean:
	rm -rf *.o Othello
