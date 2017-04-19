all:
	clang++ -g trie.cpp main.cpp board.cpp genetic/genetic.cpp genetic/aliastable.cpp -o gboggle

clean:
	rm gboggle
