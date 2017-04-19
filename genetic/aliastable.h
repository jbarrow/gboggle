/*
 *  Code to efficiently construct and sample from a multinomial distribution,
 *  will renormalize distribution if not normalized.
 */

#ifndef __ALIASTABLE_H__
#define __ALIASTABLE_H__

#include <vector>
#include <random>
#include <iostream>

class AliasTable {
  std::vector<double> distribution;
  std::vector<int> extras;

public:
  AliasTable(std::vector<double> distribution);
  ~AliasTable();

  int sample();
  void recompute(std::vector<double> distribution);
  void print();
};

#endif
