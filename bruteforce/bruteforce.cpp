#include "bruteforce.h"

BruteForce::BruteForce(int maxIteration, Trie *dict) :
  maxIteration(maxIteration), dict(dict) {
  bestScore = std::numeric_limits<int>::min();
  bestBoard = new Board(5);
}

BruteForce::~BruteForce() {
    delete bestBoard;
}

void BruteForce::search() {
  char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
  int i, score;
  i = 0;
  Board *b;
  Board *temp;
  do{
    ++i;
    b = new Board(5);
    //an permutation of b
    b->sequence_init(alphabet, ALPHABET_SIZE);
    score = b->score(dict);
    if(score > bestScore){
      bestScore = score;
      temp = bestBoard;
      bestBoard = b;
      b = temp;
    }

    delete b;
    //population[i]->print();
    //std::cout << std::endl;
  } while (i<maxIteration && std::next_permutation(alphabet,alphabet+ALPHABET_SIZE));

  std::cout << "Best performer " << bestScore << std::endl;
  std::cout << "Best sequence:" << std::endl;
  bestBoard->print();

  std::vector<std::string> words = bestBoard->solve(dict);
  std::cout << "Words found:" << std::endl;
  for (std::string w : words) std::cout << "\t" << w << std::endl;

}
