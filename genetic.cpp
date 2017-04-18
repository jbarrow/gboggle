#include "genetic.h"

Genetic::Genetic(int population_size, Trie *dict) :
  population_size(population_size), dict(dict) {
  int i;
  Board *b;
  char alphabet[] = "abcdefghiklmnopqrstuvwxyz";

  // initialize the population
  for(i = 0; i < population_size; ++i) {
    b = new Board(5);
    b->permutation_init(alphabet, ALPHABET_SIZE);

    population.push_back(b);
  }
}

Genetic::~Genetic() {
  for(int i = 0; i < population_size; ++i)
    delete population[i];
}

void Genetic::iterate() {
  int i;
  std::vector<int> scores;

  for(i = 0; i < population_size; ++i) {
    int score = population[i]->score(dict);
    scores.push_back(score);

    if(score > 0) {
      population[i]->print();
      std::cout << "Score: " << scores[i] << std::endl << std::endl;
    }
  }
}
