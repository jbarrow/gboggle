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
  initialize(n);
}

// Create an nxn boggle board with predefined values.
Board::Board(int n, char **board) : n(n), board_state(board) {}

Board::~Board() {
  delete[] this->board_state[0];
  delete[] this->board_state;
}

Board::Board(const Board& other) : n(other.n) {
  initialize(other.n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      board_state[i][j] = other.board_state[i][j];
}

void Board::initialize(int n) {
  // allocate all the pointers we'll need
  board_state = new char*[n];
  board_state[0] = new char[n * n];

  // set all the pointers at the beginning of the rows to the correct value
  for (int i = 1; i < n; ++i)
    board_state[i] = board_state[0] + i * n;

  // initialize board to all 'X' to indicate blank spaces
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      board_state[i][j] = 'X';
}

void Board::print() const {
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

Point Board::find_index(char value) const {
  int i, j;
  Point point;

  for(i = 0; i < n; ++i) {
    for(j = 0; j < n; ++j) {
      if(board_state[i][j] == value) {
        point.x = i;
        point.y = j;
        break;
      }
    }
  }

  return point;
}




// a few helpful utility functions
bool Board::has_char(char c) {
  for (int x = 0; x < n; x++)
    for (int y = 0; y < n; y++)
      if (board_state[x][y] == c) return true;

  return false;
}

char Board::get_with_wrap(int x, int y) {
  while (x < 0) x += 5;
  while (x > 4) x -= 5;
  while (y < 0) y += 5;
  while (y > 4) y -= 5;

  return board_state[x][y];
}

std::pair<int, int> Board::pos_with_wrap(int x, int y) {
  while (x < 0) x += 5;
  while (x > 4) x -= 5;
  while (y < 0) y += 5;
  while (y > 4) y -= 5;

  return std::make_pair(x, y);
}

std::set<char> Board::chars() {
  std::set<char> ret;
  for (int x = 0; x < n; x++)
    for (int y = 0; y < n; y++)
      if (board_state[x][y] != 'X')
        ret.insert(board_state[x][y]);

  return ret;
}

std::set<char> Board::chars_neighboring(int x, int y) {
  std::set<char> ret;

  for (int x_off = -1; x_off <= 1; x_off++) {
    for (int y_off = -1; y_off <= 1; y_off++) {
      if (x_off == 0 && y_off == 0) continue;

      char c = get_with_wrap(x + x_off, y + y_off);
      if (c != 'X') ret.insert(c);
    }
  }

  return ret;
}

std::set<std::pair<int, int> > Board::blank_spaces() {
  std::set<std::pair<int, int> > ret;
  for (int x = 0; x < n; x++)
    for (int y = 0; y < n; y++)
      if (board_state[x][y] == 'X')
        ret.insert(std::make_pair(x, y));

  return ret;
}

std::set<std::pair<int, int> > Board::blank_spaces_neighboring(int x, int y) {
  std::set<std::pair<int, int> > ret;

  for (int x_off = -1; x_off <= 1; x_off++) {
    for (int y_off = -1; y_off <= 1; y_off++) {
      if (x_off == 0 && y_off == 0) continue;

      auto c = pos_with_wrap(x + x_off, y + y_off);
      if (board_state[c.first][c.second] == 'X')
        ret.insert(c);
    }
  }

  return ret;
}
