#include "trie.h"
#include "board.h"
#include "genetic/genetic.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

int main(int argc, char **argv) {
  if(argc != 5) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./gboggle [iterations] [population] [seed] [dictionary]" << std::endl;
    exit(0);
  }

  // seed the random number generator
  if(atoi(argv[3]) < 0)
    std::srand(std::time(NULL));
  else
    std::srand(atoi(argv[3]));
  // get the iterations
  int iterations = atoi(argv[1]);
  // create a new trie, and load our dictionary into it
  Trie *trie = new Trie();
  read_dictionary(argv[4], trie);

  //Board *b = new Board(5);
  //char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
  //b->permutation_init(alphabet, ALPHABET_SIZE);
  //b->print();
  //std::cout << "Final Score: " << b->score(trie) << std::endl;

  Genetic *g = new Genetic(atoi(argv[2]), trie);
  for(int i = 0; i < iterations; ++i) g->iterate();

  // clean up the trie
  delete trie;

}
