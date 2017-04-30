#include "trie.h"
#include <ctype.h>
#include <string>

// the bit of code that 'hashes' the given character, ensures lowercase letters
inline int char_to_index(char c) { return (int)tolower(c) - (int)'a' - ((int)tolower(c) >= (int)'j'); }

Trie::~Trie() {
  // Destructor causes a recursive deletion of the children in the trie.
  for(int i = 0; i < ALPHABET_SIZE; ++i) {
    if(children[i] != NULL) delete children[i];
  }
}

void Trie::insert(std::string word) {
  // Insert a new word into the trie.
  Trie *node = this;
  for(int i = 0; i < word.length(); ++i) {
    // skipping non-valid characters
    if(char_to_index(word[i]) >= ALPHABET_SIZE || char_to_index(word[i]) < 0)
      return;
    // if the child doesn't yet exist, create it
    if(node->children[char_to_index(word[i])] == NULL) {
      node->children[char_to_index(word[i])] = new Trie();
    }
    // and select it as the current node we're looking at
    node = node->children[char_to_index(word[i])];
  }
  // set the valid flag so we know the word exists
  node->valid = true;
}

Trie* Trie::prefix_search(std::string prefix) {
  // Check if a prefix exists in the trie, and if it does, return a pointer
  // to the final node of the prefix.
  Trie *node = this;
  for(int i = 0; i < prefix.length(); ++i) {
    node = node->children[char_to_index(prefix[i])];
    // if it doesn't exist, return NULL
    if(node == NULL) return NULL;
  }
  return node;
}

bool Trie::search(std::string word) {
  // Returns a boolean value whether or not a given word exists in the trie.
  Trie *node = prefix_search(word);
  if(node == NULL) return false;
  return node->valid;
}

void read_dictionary(std::string filename, Trie *root) {
  std::cout << "Reading: " << filename << std::endl;
  // open the dictionary file (one word per line)
  std::ifstream infile(filename);
  std::string line;
  int count = 0;

  while(std::getline(infile, line)) {
    // insert the word into the trie
    root->insert(line);
    ++count;
  }

  std::cout << "Read " << count << " words" << std::endl;
}

void read_dictionary(std::string filename, std::vector<std::string> *vec) {
  std::cout << "Reading: " << filename << std::endl;

  std::ifstream infile(filename);
  std::string line;
  vec->clear();
  
  while(std::getline(infile, line)) vec->push_back(line);

  std::cout << "Read " << (int)vec->size() << " words" << std::endl;
}

