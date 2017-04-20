CC=g++
CFLAGS=-g -Wall

all: trie.o board.o bruteforce.o genetic.o aliastable.o
	$(CC) $(CFLAGS) -o gboggle trie.o board.o bruteforce.o genetic.o aliastable.o main.cpp

bruteforce.o: board.o trie.o
	$(CC) $(CFLAGS) -c bruteforce/bruteforce.cpp trie.o board.o

genetic.o: board.o trie.o aliastable.o
	$(CC) $(CFLAGS) -c genetic/genetic.cpp trie.o board.o aliastable.o

board.o: trie.o
	$(CC) $(CFLAGS) -c board.cpp trie.o

trie.o:
	$(CC) $(CFLAGS) -c trie.cpp

aliastable.o:
	$(CC) $(CFLAGS) -c genetic/aliastable.cpp


clean:
	rm gboggle
