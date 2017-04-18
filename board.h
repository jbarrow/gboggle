#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <vector>
#include "trie.h"

class Board {
  char **board_state;
  int n;

public:
  Board(int n);
  Board(int n, char** board);
  ~Board();

  int score(Trie *dict);
  int word_score(std::string word);
  std::vector<std::string> solve(Trie *dict);
  void search(Trie*, std::string, int, int, std::vector<bool>, std::vector<std::string> &found);

  void permutation_init(char *alphabet, int alphabet_size);
  void random_init(char *alphabet, int alphabet_size);
  void print();
};

#endif
