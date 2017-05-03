#include "genetic.h"

Genetic::Genetic(int population_size, Trie *dict) :
  population_size(population_size), dict(dict) {
  int i;
  Board *b;
  char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
  std::set<char> alphabetSet(alphabet, alphabet+ALPHABET_SIZE);


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

void Genetic::pmx_2d_crossover(const Board *p1, const Board *p2, Board *child) {
  int i, j;
  int i1, j1, i2, j2, tmp;
  Point point;
  char val;
  std::set<char> unused(alphabetSet);
  std::set<char> inChild;
  std::vector<std::pair<Point, char>> fixList;
  // fill in the child with discernable char
  for(i = 0; i < child->n; ++i)
    for(j = 0; j < child->n; ++j)
      child->board_state[i][j] = '0';

  // compute swath of p1
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> distribution(0, p1->n-1);

  i1 = distribution(rng);
  i2 = distribution(rng);
  j1 = distribution(rng);
  j2 = distribution(rng);

  if(j1 > j2){
    temp = j1;
    j1 = j2;
    j2 = temp;
  }

  if(i1 > i2){
    temp = i1;
    i1 = i2;
    i2 = temp;
  }

  // copy swath from Parent1 to update child
  for(i = i1; i <= i2; ++i){
    for(j = j1; j <= j2; ++j){
      child->board_state[i][j] = p1->board_state[i][j];
      inChild.insert(p1->board_state[i][j]);
    }
  }
  // compute set of unused values
  unused.erase(inChild.begin(), inChild.end);

  // Figure out which values from Parent2's swath were not put in the child from Parent1
  for(i = i1; i <= i2; ++i){
    for(j = j1; j <= j2; ++j){
      if(inChild.find(p2->board_state[i][j]) == inChild.end()){
        point.x = i;
        point.y = j;
        val = p2->board_state[i][j];
        fixList.push_back(std::pair <Point,char> fix (index,val));
      }
    }
  }

  // Fix all in fix list
  for (std::vector<std::pair<Point, char>>::iterator it = fixList.begin(); it != fixList.end(); ++it){
    point = it->first;
    val = it->second;
    // find position (in parent2) of value in Pair's position in child
    point = p2->find_index(child->board_state[point.x][point.y]);
    //If position is taken in child already, find position of item that is in its spot in parent 2
    //Otherwise, put it there
    while (child->board_state[point.x][point.y] != '0'){
      point = p2->find_index(child->board_state[point.x][point.y]);
    }

    // If this is reached, we drop the Pair.value in Position
    child->board_state[point.x][point.y] = val;
    inChild.insert(val);
  }

  //Drop the remaining values in
  for(i = 0; i < child->n; ++i){
    for(j = 0; j < child->n; ++j){
      if(inChild.find(p2->board_state[i][j]) == inChild.end()){
        child->board_state[i][j] = p2->board_state[i][j];
        // is this needed?
        inChild.insert(p2->board_state[i][j]);
      }
    }
  }
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
