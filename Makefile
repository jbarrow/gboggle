all:
	clang++ trie.cpp main.cpp board.cpp genetic.cpp -o gboggle

clean:
	rm gboggle
