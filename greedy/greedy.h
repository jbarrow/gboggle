#ifndef __GREEDY_H__
#define __GREEDY_H__

#include "../board.h"
#include "adjacencymatrix.h"

#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

class Greedy {
    std::vector<std::string> wordlist;
    AdjacencyMatrix* adj;

    public:
        Greedy(std::vector<std::string> wordlist);
        ~Greedy();

        void search();
};

struct string_length_compare {
    bool operator()(const std::string& first, const std::string& second) {
        return first.size() > second.size();
    }
};

#endif
