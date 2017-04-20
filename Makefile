all:
	clang++ trie.cpp main.cpp board.cpp bruteforce/bruteforce.cpp genetic/genetic.cpp genetic/aliastable.cpp -o gboggle

clean:
	rm gboggle
