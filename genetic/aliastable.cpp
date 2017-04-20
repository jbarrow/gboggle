#include "aliastable.h"

AliasTable::AliasTable(std::vector<double> dist) {
  // pre-populate our K-vector
  for(int i = 0; i < dist.size(); ++i)
    extras.push_back(i);

  recompute(dist);
}

AliasTable::~AliasTable() {}

int AliasTable::sample() {
  std::random_device rd;
  std::mt19937 rng(rd());

  std::uniform_int_distribution<int> ix_sampler(0, distribution.size()-1);
  std::uniform_real_distribution<double> vl_sampler(0, 1.0);

  int ix = ix_sampler(rng);
  double val = vl_sampler(rng);

  return val < distribution[ix] ? ix : extras[ix];
}

void AliasTable::recompute(std::vector<double> dist) {
  int i, ux, ox;
  double sum = 0.0;
  std::vector<int> overfull, underfull;

  // normalize the distribution
  for(i = 0; i < dist.size(); ++i) sum += dist[i];
  for(i = 0; i < dist.size(); ++i) {
    distribution.push_back((dist[i]) / sum * dist.size());
    //std::cout << dist[i] << ", " << distribution[i] << std::endl;
  }

  // compute over- and under-full bins
  for(i = 0; i < distribution.size(); ++i) {
    if(distribution[i] > 1.0)
      overfull.push_back(i);
    else if(distribution[i] < 1.0)
      underfull.push_back(i);
  }

  while(underfull.size() > 0 && overfull.size() > 0) {
    ux = underfull.back();
    ox = overfull.back();

    underfull.pop_back();

    extras[ux] = ox;
    distribution[ox] -= 1. - distribution[ux];

    if(overfull.size() > 0 && distribution[ox] < 1.0) {
      overfull.pop_back();
      underfull.push_back(ox);
    }

    //std::cout << "Fixed: " << ux << std::endl;
  }
}

void AliasTable::print() {
  for(int i = 0; i < distribution.size(); ++i)
    std::cout << extras[i] << "\t";
  std::cout << std::endl;

  for(int i = 0; i < distribution.size(); ++i)
    std::cout << distribution[i] << "\t";
  std::cout << std::endl;
}
