#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "../board.h"
#include "../trie.h"
#include "aliastable.h"

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <set>
#include <utility>

class Genetic {
  int population_size;
  double max;
  Trie *dict;
  std::set<char> alphabet_set;
  void pmx_2d_crossover(const Board *p1, const Board*p2, Board *update, std::mt19937 &rng);
  void mutate(const Board *original, Board *update, std::mt19937 &rng);
  void select(const Board *original, Board *update);


public:
  std::vector<Board*> *population;
  std::vector<Board*> *buffer;
  std::vector<double> *scores;
  int tournament_selection(AliasTable* table, std::vector<double> *scores, std::mt19937 &rng);
  void build_child(Board *child, AliasTable *table, std::vector<double> *scores, std::mt19937 &rng);

  Genetic(int population_size, Trie *dict);
  ~Genetic();

  void iterate();
};

#endif
