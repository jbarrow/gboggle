#include "trie.h"
#include "board.h"
#include "genetic/genetic.h"
#include "bruteforce/bruteforce.h"

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

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

int main(int argc, char **argv) {
  if(argc != 5 && argc != 4) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./gboggle [iterations] [population] [seed] [dictionary]" << std::endl;
    std::cout << "\t./gboggle [maxIteration] [seed] [dictionary]" << std::endl;
    exit(0);
  }

  if(argc == 5) {
    // seed the random number generator
    if(atoi(argv[3]) < 0)
      std::srand(std::time(NULL));
    else
      std::srand(atoi(argv[3]));
    // get the iterations
    int iterations = atoi(argv[1]);
    char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
    // create a new trie, and load our dictionary into it
    Trie *trie = new Trie();
    read_dictionary(argv[4], trie);

    //Board *b = new Board(5);
    //b->permutation_init(alphabet, ALPHABET_SIZE);
    //b->print();
    //std::cout << "Final Score: " << b->score(trie) << std::endl;

    Genetic *g = new Genetic(atoi(argv[2]), trie);

    for(int i = 0; i < iterations; ++i) {
      g->iterate();
    }

    // clean up the trie
    delete trie;

  } else {
    // seed the random number generator
    if(atoi(argv[2]) < 0)
      std::srand(std::time(NULL));
    else
      std::srand(atoi(argv[2]));
    // get the iterations
    int maxIteration = atoi(argv[1]);
    char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
    // create a new trie, and load our dictionary into it
    Trie *trie = new Trie();
    read_dictionary(argv[3], trie);

    BruteForce *brute = new BruteForce(maxIteration, trie);
    brute->search();
    
    // clean up the trie
    delete trie;
  }
}
