CC=g++
CFLAGS=-g -Wall -std=c++11 #-O3 -fopenmp

all: trie.o board.o bruteforce.o genetic.o aliastable.o adjacencymatrix.o greedy.o
	$(CC) $(CFLAGS) -o gnboggle trie.o board.o genetic.o aliastable.o genetic.cpp
	$(CC) $(CFLAGS) -o bfboggle trie.o board.o bruteforce.o bruteforce.cpp
	$(CC) $(CFLAGS) -o grboggle trie.o board.o greedy.o adjacencymatrix.o greedy.cpp

genetic: genetic.o trie.o board.o aliastable.o
	$(CC) $(CFLAGS) -o gnboggle trie.o board.o genetic.o aliastable.o genetic.cpp

bruteforce: trie.o board.o bruteforce.o
	$(CC) $(CFLAGS) -o bfboggle trie.o board.o bruteforce.o bruteforce.cpp

greedy: trie.o board.o greedy.o adjacencymatrix.o
	$(CC) $(CFLAGS) -o grboggle trie.o board.o greedy.o adjacencymatrix.o greedy.cpp

greedy.o: board.o trie.o
	$(CC) $(CFLAGS) -c greedy/greedy.cpp trie.o board.o

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

adjacencymatrix.o:
	$(CC) $(CFLAGS) -c greedy/adjacencymatrix.cpp

clean:
	rm *.o
	rm grboggle gnboggle bfboggle
