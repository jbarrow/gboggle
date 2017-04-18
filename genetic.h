#ifndef __GENETIC_H__
#define __GENETIC_H__

#include "board.h"

class Genetic {
  int population;

public:
  Genetic(int population_size);
  ~Genetic();

  void iterate();
};

#endif
