#include "trie.h"
#include "board.h"
#include "genetic/genetic.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <mpi.h>


void swap(Genetic *g, int swap_population, std::mt19937 &rng, AliasTable *table) {
  int i, my_rank, world_size;
  char** value;
  std::vector<int> population_to_swap;
  MPI_Status status;

  // tournament select which ones to swap
  for(i = 0; i < swap_population; ++i) {
    population_to_swap.push_back(g->tournament_selection(table, g->scores, rng));
  }

  // MPI calls
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(my_rank % 2 == 0) {
    for(i = 0; i < swap_population; ++i) {
      value = (*(g->population))[population_to_swap[i]]->board_state;
      MPI_Send(&(value[0][0]), 25, MPI_CHAR, (my_rank+1)%world_size, my_rank, MPI_COMM_WORLD);
    }
  }

  for(i = 0; i < swap_population; ++i) {
    int recv_target = (my_rank + world_size - 1) % world_size;
    value = (*(g->buffer))[i]->board_state;
    MPI_Recv(&(value[0][0]), 25, MPI_CHAR, recv_target, recv_target, MPI_COMM_WORLD, &status);
    std::cout << "RECEIVED BOARD:::::" << std::endl;
    (*(g->buffer))[i]->print();
  }

  if(my_rank % 2 == 1) {
    for(i = 0; i < swap_population; ++i) {
      value = (*(g->population))[population_to_swap[i]]->board_state;
      MPI_Send(&(value[0][0]), 25, MPI_CHAR, (my_rank+1)%world_size, my_rank, MPI_COMM_WORLD);
    }
  }
}

int main(int argc, char **argv) {
  if(argc <= 5) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./islands [iterations] [swaps] [population] [swap_population] [dictionary]" << std::endl;
    exit(0);
  }

  // get the iterations
  int i;
  int iterations = atoi(argv[1]);
  int swaps = atoi(argv[2]);
  int population = atoi(argv[3]);
  int swap_population = atoi(argv[4]);
  // create a new trie, and load our dictionary into it
  Trie *trie = new Trie();
  read_dictionary(argv[5], trie);

  //initialize MPI
  MPI_Init(&argc, &argv);

  //Create the swap population
  std::mt19937 rng(std::random_device{}());
  Genetic *g = new Genetic(population, trie);
  std::vector<Board*> *tmp;

  for(int j = 0; j < swaps; ++j) {
    for(int i = 0; i < iterations; ++i) g->iterate();

    // swap genetic material with island
    std::cout << "***** SWAPPING" << std::endl << std::endl;
    AliasTable* table = new AliasTable(*(g->scores));
    swap(g, swap_population, rng, table);

    for(i = swap_population; i < population; ++i) {
      g->build_child((*(g->buffer))[i], table, g->scores, rng);
    }

    tmp = g->population;
    g->population = g->buffer;
    g->buffer = tmp;

    delete table;
  }

  MPI_Finalize();

  // clean up the trie
  delete g;
  delete trie;
}
