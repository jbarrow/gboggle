#ifndef __TRIE_H__
#define __TRIE_H__

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define ALPHABET_SIZE 25

class Trie {
  // represent the children as a length 26 array -- effectively a hash table
  Trie *children[ALPHABET_SIZE];

public:
  Trie() : valid(false) { for(int i = 0; i < ALPHABET_SIZE; ++i) children[i] = NULL; }
  ~Trie();

  // trie operations
  Trie* prefix_search(std::string prefix);
  void insert(std::string word);
  bool search(std::string word);

  bool valid;
};

void read_dictionary(std::string filename, Trie *root);
void read_dictionary(std::string filename, std::vector<std::string> *vec);

#endif
