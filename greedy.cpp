#include "trie.h"
#include "board.h"
#include "greedy/greedy.h"
#include <iostream>

int main(int argc, char **argv) {
  if(argc != 2) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./greedy [dictionary]" << std::endl;
    exit(0);
  }

  // load our dictionary into a vector of strings
  std::vector<std::string> wordlist;
  read_dictionary(argv[1], &wordlist);

  Greedy *g = new Greedy(wordlist);
  g->search();
  delete g;
}
