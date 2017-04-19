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
    buffer.push_back(b);
  }
}

Genetic::~Genetic() {
  for(int i = 0; i < population_size; ++i) {
    delete population[i];
    delete buffer[i];
  }
}

void mutate(Board *original, Board *update) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::uniform_real_distribution<double> maybe_mutate(0, 1.0);

  if(maybe_mutate(rng) > 0.6) {
    std::uniform_int_distribution<int> distribution(0, original->n-1);
    int i1, j1, i2, j2;

    i1 = distribution(rng);
    i2 = distribution(rng);
    j1 = distribution(rng);
    j2 = distribution(rng);

    //std::cout << "Chosen index: " << i1 << " " << i2 << " " << j1 << " " << j2 << std::endl;

    char tmp = update->board_state[i1][j1];
    update->board_state[i1][j1] = update->board_state[i2][j2];
    update->board_state[i2][j2] = tmp;
  }
}

void Genetic::iterate() {
  int i, score, index;
  std::vector<double> scores;

  for(i = 0; i < population_size; ++i) {
    score = population[i]->score(dict);
    scores.push_back((double)score);
    //population[i]->print();
    //std::cout << std::endl;
  }

  double max = *max_element(scores.begin(), scores.end());

  std::cout << "Best performer " << max << std::endl;

  AliasTable* table = new AliasTable(scores);
  //table->print();

  for(i = 0; i < population_size; ++i) {
    index = table->sample();

    mutate(population[index], buffer[i]);
  }

  population = buffer;
}
