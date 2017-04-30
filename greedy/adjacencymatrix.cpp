#include "adjacencymatrix.h"

inline int char_to_index(char c) { return (int)tolower(c) - (int)'a' - ((int)tolower(c) >= (int)'j'); }

// default constructor
AdjacencyMatrix::AdjacencyMatrix() {
    // initialize 25x25 adjacency matrix with all false
    mat = new bool*[25];
    for (int i = 0; i < 25; i++) {
        mat[i] = new bool[25];

        for (int j = 0; j < 25; j++) mat[i][j] = false;
    }
}

// "copy constructor" which adds new word to matrix
AdjacencyMatrix::AdjacencyMatrix(AdjacencyMatrix* old, std::string to_add) : AdjacencyMatrix() {
    // copy old matrix
    for (int i = 0; i < 25; i++)
        for (int j = 0; j < 25; j++)
            mat[i][j] = old->mat[i][j];

    // add word [to_add] to adjacency matrix
    for (int i = 1; i < to_add.length(); i++) {
        char prev = to_add[i - 1];
        char curr = to_add[i];

        // get indices in matrix
        int prev_idx = char_to_index(prev);
        int curr_idx = char_to_index(curr);

        // ignore non-valid characters
        if (prev_idx >= 25 || prev_idx < 0) continue;
        if (curr_idx >= 25 || curr_idx < 0) continue;

        // add edge between characters
        mat[prev_idx][curr_idx] = true;
        mat[curr_idx][prev_idx] = true;
    }
}

// destructor
AdjacencyMatrix::~AdjacencyMatrix() {
    delete[] mat;
}

// check for validity
bool AdjacencyMatrix::is_valid() {
    // valid if the sum of all rows and columns <= 8 (i.e. a letter has
    // no more than 8 neighbors)

    // check vertically first
    for (int x = 0; x < 25; x++) {
        int sum = 0;

        for (int y = 0; y < 25; y++)
            if (mat[x][y]) sum++;

        if (sum > 8) return false;
    }

    // then check horizontally
    for (int y = 0; y < 25; y++) {
        int sum = 0;

        for (int x = 0; x < 25; x++)
            if (mat[x][y]) sum++;

        if (sum > 8) return false;
    }

    // if no invalidities were found, our matrix is valid
    return true;
}


