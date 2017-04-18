#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "board.h"
#include "trie.h"

#include <vector>
#include <iostream>

class Genetic {
  int population_size;
  std::vector<Board*> population;
  Trie *dict;

public:
  Genetic(int population_size, Trie *dict);
  ~Genetic();

  void iterate();
};

#endif
