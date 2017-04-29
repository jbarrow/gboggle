#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "../board.h"
#include "../trie.h"
#include "aliastable.h"

#include <vector>
#include <iostream>
#include <random>

class Genetic {
  int population_size;
  std::vector<Board*> population;
  std::vector<Board*> buffer;
  Trie *dict;

  int tournament_selection(AliasTable* table, std::vector<double> scores);
  void pmx_2d_crossover(const Board *p1, const Board*p2, Board *update);
  void mutate(const Board *original, Board *update);

public:
  Genetic(int population_size, Trie *dict);
  ~Genetic();

  void iterate();
};

#endif
