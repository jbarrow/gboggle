#include "trie.h"
#include "board.h"
#include "greedy/greedy.h"
#include <iostream>

int main(int argc, char **argv) {
  if(argc != 2) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./bruteforce [dictionary]" << std::endl;
    exit(0);
  }

  // create a new trie, and load our dictionary into it
  Trie *trie = new Trie();
  read_dictionary(argv[1], trie);

  // TODO: Sri, you can put your code here.

  // clean up the trie
  delete trie;
}
