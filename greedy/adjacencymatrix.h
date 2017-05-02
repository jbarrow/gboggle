#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include <vector>
#include <string>
#include <set>
#include <map>
#include <utility>

#include "../board.h"

class AdjacencyMatrix {
    public:
        AdjacencyMatrix();
        AdjacencyMatrix(AdjacencyMatrix* old, std::string to_add);
        ~AdjacencyMatrix();

        bool is_valid();
        Board* to_board();
        bool fill_board(Board* board, std::map<char, std::set<char>>& constraints,
                        std::set<char>& placed, char to_place,
                        std::set<std::pair<int, int>> possible_positions,
                        int iter);
        void print();
        std::map<char, std::set<char>> to_map(bool include_empty);

        bool **mat;
};

#endif
