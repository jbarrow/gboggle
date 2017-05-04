#include "trie.h"
#include "board.h"
#include "genetic/genetic.h"
#include "islands/island.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

void swap() {}

int main(int argc, char **argv) {
  if(argc != 5) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./gboggle [iterations] [swaps] [population] [dictionary]" << std::endl;
    exit(0);
  }

  // get the iterations
  int iterations = atoi(argv[1]);
  int swaps = atoi(argv[2]);
  int population = atoi(argv[3]);
  // create a new trie, and load our dictionary into it
  Trie *trie = new Trie();
  read_dictionary(argv[4], trie);

  for(int j = 0; j < swaps; ++j) {
    Genetic *g = new Genetic(population, trie);
    for(int i = 0; i < iterations; ++i) g->iterate();

    // sample island from number of mpi processes

    // swap genetic material with island
    swap();
  }

  // clean up the trie
  delete trie;
}
