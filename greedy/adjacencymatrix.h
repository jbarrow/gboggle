#ifndef __ADJACENCYMATRIX_H__
#define __ADJACENCYMATRIX_H__

#include <vector>
#include <string>

class AdjacencyMatrix {
    bool **mat;

    public:
        AdjacencyMatrix();
        AdjacencyMatrix(AdjacencyMatrix* old, std::string to_add);
        ~AdjacencyMatrix();

        bool is_valid();
};

#endif
