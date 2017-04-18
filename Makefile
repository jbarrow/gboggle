all:
	clang++ trie.cpp main.cpp board.cpp genetic.cpp -o boggle

clean:
	rm boggle
