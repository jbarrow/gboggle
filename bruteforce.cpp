#include "trie.h"
#include "board.h"
#include "bruteforce/bruteforce.h"

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

int main(int argc, char **argv) {
  if(argc != 4) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t./bruteforce [maxIteration] [seed] [dictionary]" << std::endl;
    exit(0);
  }

  if(argc == 4) {
    // seed the random number generator
    if(atoi(argv[2]) < 0)
      std::srand(std::time(NULL));
    else
      std::srand(atoi(argv[2]));
    // get the iterations
    int maxIteration = atoi(argv[1]);
    // create a new trie, and load our dictionary into it
    Trie *trie = new Trie();
    read_dictionary(argv[3], trie);

    BruteForce *brute = new BruteForce(maxIteration, trie);
    brute->search();

    // clean up the trie
    delete trie;
  }
}
