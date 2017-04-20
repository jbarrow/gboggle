#ifndef __BRUTEFORCE_H__
#define __BRUTEFORCE_H__

#include "../board.h"
#include "../trie.h"

#include <vector>
#include <iostream>
#include <random>
#include <limits>
#include <algorithm>

class BruteForce {
  int maxIteration;
  int bestScore;
  Board *bestBoard;
  Board *currentBoard;
  Trie *dict;

public:
  BruteForce(int maxIteration, Trie *dict);
  ~BruteForce();

  void search();
};

#endif
