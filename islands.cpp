#include "trie.h"
#include "board.h"
#include "genetic/genetic.h"
#include "islands/island.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

/*
 * TODO: implement swap within genetic.cpp -- need to take in the contents of a board
 *        and overwrite an old one (perhaps chosen from bottom of the population?)
 * TODO: implement MPI calls in swap that pass boards from one island to another
 * TODO: sample swap_population boards and feed them into swap
 */

void swap() {
  // handle all MPI calls in here
}

int main(int argc, char **argv) {
  if(argc != 5) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./gboggle [iterations] [swaps] [population] [swap_population] [dictionary]" << std::endl;
    exit(0);
  }

  // get the iterations
  int iterations = atoi(argv[1]);
  int swaps = atoi(argv[2]);
  int population = atoi(argv[3]);
  int swap_population = atoi(argv[4]);
  // create a new trie, and load our dictionary into it
  Trie *trie = new Trie();
  read_dictionary(argv[5], trie);

  for(int j = 0; j < swaps; ++j) {
    Genetic *g = new Genetic(population, trie);
    for(int i = 0; i < iterations; ++i) g->iterate();

    // to get boards, use the Genetic class to randomly sample population members
    for(int k = 0; k < swap_population; ++k) {
      g->tournament_selection(/* NEED TO PASS RNG */);
      // get the boards
    }

    // sample island from number of mpi processes
    // set up a random_uniform_int from 0 to number of processes and select one

    // swap genetic material with island
    swap();
  }

  // clean up the trie
  delete trie;
}
