#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include <vector>
#include <string>
#include <set>

#include "../board.h"

class AdjacencyMatrix {
    public:
        AdjacencyMatrix();
        AdjacencyMatrix(AdjacencyMatrix* old, std::string to_add);
        ~AdjacencyMatrix();

        bool is_valid();
        Board* to_board();
        bool fill_board(char** board, std::set<char>& to_add, int letter_idx, int x, int y, int iter);

        bool **mat;
};

#endif
