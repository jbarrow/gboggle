#include "board.h"
#include "trie.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <random>
#include <cstdlib>

Board::Board(int n) : n(n) {
  // Create an nxn boggle board, and randomly initialize it.
  int i;
  // allocate all the pointers we'll need
  board_state = new char*[n*sizeof(char*)];
  board_state[0] = new char[n*n*sizeof(char)];
  // set all the pointers at the beginning of the rows to the correct value
  for(i = 1; i < n; ++i)
    board_state[i] = board_state[0] + i * n;
}

// Create an nxn boggle board with predefined values.
Board::Board(int n, char **board) : n(n), board_state(board) {}

Board::~Board() {
  delete[] this->board_state;
}

void Board::print() {
  int i, j;
  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j) {
      std::cout << board_state[i][j] << "  ";
    }
    std::cout << std::endl;
  }
}

int seeded_random(int i) {
  return std::rand()%i;
}

void Board::permutation_init(char *alphabet, int alphabet_size) {
  if(alphabet_size != n * n) {
    std::cout << "Board permutation and alphabet size aren't compatible." << std::endl;
    std::exit(0);
  }

  std::vector<char> alphabet_vector;
  for(int i = 0; i < alphabet_size; ++i) alphabet_vector.push_back(alphabet[i]);
  std::random_shuffle(alphabet_vector.begin(), alphabet_vector.end(), seeded_random);

  for(int j = 0; j < n; ++j)
    for(int i = 0; i < n; ++i)
      board_state[i][j] = alphabet_vector[i*n+j];
}

void Board::sequence_init(char *alphabet, int alphabet_size) {
  if(alphabet_size != n * n) {
    std::cout << "Board permutation and alphabet size aren't compatible." << std::endl;
    std::exit(0);
  }

  for(int j = 0; j < n; ++j)
    for(int i = 0; i < n; ++i)
      board_state[i][j] = alphabet[i*n+j];
}

void Board::random_init(char *alphabet, int alphabet_size) {}

int Board::score(Trie *dict) {
  // Given the current board state, return the total score for all words in a
  // dictionary.
  std::vector<std::string> words = solve(dict);
  int score = 0, count = 0;
  // add the score for every word in the list
  for(std::vector<std::string>::size_type i = 0; i != words.size(); ++i) {
    score += word_score(words[i]);
    ++count;

    if(count >= 20) break;
  }

  return score;
}

int Board::word_score(std::string word) {
  // Get the score for a given word. Note that the score is only dependent on
  // the length of the word.
  /*
  switch(word.length()) {
    case 1:
    case 2:
      return 0;
    case 3:
    case 4:
      return 1;
    case 5:
      return 2;
    case 6:
      return 3;
    case 7:
      return 5;
    default:
      return 11;
  }
  */
  return word.length();
}

void Board::search(Trie *dict, std::string prefix, int x, int y, std::vector<bool> searched, std::vector<std::string> &found) {
  int i, j;
  // update the current string
  prefix += board_state[x][y];
  // check if it's a valid node
  Trie *node = dict->prefix_search(prefix);
  // check if this is in the dictionary
  if (node != NULL) {
    if (node->valid) found.push_back(prefix);
    // mark the current node as completed
    searched[x*n+y] = true;
    // iterate over the children and search them
    for(i = x-1; i <= x+1; ++i)
      for(j = y-1; j <= y+1; ++j)
        // non-wrapping
        if(i >= 0 && i < n && j >= 0 && j < n)
          search(dict, prefix, i, j, searched, found);
        // wrapping
        //search(dict, prefix, (i+n)%n, (j+n)%n, searched, found);
  }
}

std::vector<std::string> Board::solve(Trie *dict) {
  // Given the current board state, compute all the words present on the board
  std::vector<std::string> words;
  std::vector<bool> searched;
  int i, j;
  compare c;

  for(i = 0; i < n * n; ++i) searched.push_back(false);

  // iterate over every character present on the board, add it to the stack
  for(i = 0; i < n; ++i)
    for(j = 0; j < n; ++j)
      search(dict, "", i, j, searched, words);

  std::sort(words.begin(), words.end(), c);

  return words;
}
