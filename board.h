#ifndef __BOARD_H__
#define __BOARD_H__

#include <string>
#include <vector>
#include <set>
#include <utility>
#include "trie.h"

struct Point {
  int x, y;
};

class Board {

public:
  int n;
  char **board_state;

  Board(int n);
  Board(int n, char** board);
  ~Board();
  Board(const Board& other);

  int score(Trie *dict);
  int word_score(std::string word);
  std::vector<std::string> solve(Trie *dict);
  void search(Trie*, std::string, int, int, std::vector<bool>, std::vector<std::string> &found);

  void sequence_init(char *alphabet, int alphabet_size);
  void permutation_init(char *alphabet, int alphabet_size);
  void random_init(char *alphabet, int alphabet_size);
  void print();

  Point find_index(char value);

  bool has_char(char c);

  char get_with_wrap(int x, int y);
  std::pair<int, int> pos_with_wrap(int x, int y);

  std::set<char> chars();
  std::set<char> chars_neighboring(int x, int y);
  std::set<std::pair<int, int>> blank_spaces();
  std::set<std::pair<int, int>> blank_spaces_neighboring(int x, int y);
};

struct compare {
  bool operator()(const std::string& fst, const std::string& snd) {
    return fst.size() > snd.size();
  }
};

#endif
