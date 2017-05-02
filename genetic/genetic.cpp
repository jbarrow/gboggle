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

int Genetic::tournament_selection(AliasTable* table, std::vector<double> scores) {
  // perform tournament selection on two candidate members of the population
  int c_one = table->sample();
  int c_two = table->sample();

  return scores[c_one] > scores[c_two] ? c_one : c_two;
}

void Genetic::pmx_2d_crossover(const Board *p1, const Board *p2, Board *update) {
  int i, j;
  int i1, j1, i2, j2;
  char val;
  std::vector<Point> unused;
  // fill in the child with discernable char
  for(i = 0; i < update->n; ++i)
    for(j = 0; j < update->n; ++j)
      update->board_state[i][j] = '0';

  // compute swath of p1
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> distribution(0, p1->n-1);

  i1 = distribution(rng);
  i2 = distribution(rng);
  j1 = distribution(rng);
  j2 = distribution(rng);

  // copy swath to update
  for(i = std::min(i1, i2); i <= std::max(i1, i2); ++i)
    for(j = std::min(j1, j2); j <= std::max(j1, j2); ++j)
      update->board_state[i][j] = p1->board_state[i][j];

  // compute list of unused values
  // p2->get_index(char) returns a point
}

void Genetic::mutate(const Board *original, Board *update) {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::uniform_real_distribution<double> maybe_mutate(0, 1.0);

  if(maybe_mutate(rng) > 0.3) {
    std::uniform_int_distribution<int> distribution(0, original->n-1);
    int i1, j1, i2, j2;

    i1 = distribution(rng);
    i2 = distribution(rng);
    j1 = distribution(rng);
    j2 = distribution(rng);

    char tmp = update->board_state[i1][j1];
    update->board_state[i1][j1] = update->board_state[(i1+1)%original->n][(j1+1)%original->n];
    update->board_state[(i1+1)%original->n][(j1+1)%original->n] = tmp;
  }
}

void Genetic::iterate() {
  int i, score, index;
  std::vector<double> scores;

  for(i = 0; i < population_size; ++i) {
    score = population[i]->score(dict);
    scores.push_back((double)score);
  }

  double max = *max_element(scores.begin(), scores.end());

  std::cout << "Best performer " << max << std::endl;

  AliasTable* table = new AliasTable(scores);

  for(i = 0; i < population_size; ++i) {
    index = tournament_selection(table, scores);

    mutate(population[index], buffer[i]);
  }

  population = buffer;
}
