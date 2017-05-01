#include "adjacencymatrix.h"
#include <stack>
#include <algorithm>

inline int char_to_index(char c) { return (int)tolower(c) - (int)'a' - ((int)tolower(c) >= (int)'j'); }
inline char index_to_char(int i) { return 'a' + i + (int)(i >= (int)('j' - 'a')); }

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

        if (prev == curr) continue;

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


std::set<char> board_neighbors(char** board, int x, int y) {
    std::set<char> ret;

    for (int x_off = -1; x_off <= 1; x_off++) {
        for (int y_off = -1; y_off <= 1; y_off++) {
            if (x_off == 0 && y_off == 0) continue;

            int x_pos = x + x_off;
            int y_pos = y + y_off;

            if (x_pos < 0) x_pos += 5;
            if (x_pos > 4) x_pos -= 5;
            if (y_pos < 0) y_pos += 5;
            if (y_pos > 4) y_pos -= 5;

            if (board[x_pos][y_pos] != 'X')
                ret.insert(board[x_pos][y_pos]);
        }
    }

    return ret;
}

std::set<char> adj_mat_neighbors(AdjacencyMatrix* adj, char c) {
    std::set<char> ret;

    int idx = char_to_index(c);
    bool* adj_neighbors = adj->mat[idx];
    for (int i = 0; i < 25; i++) {
        if (adj_neighbors[i]) {
            ret.insert(index_to_char(i));
        }
    }

    return ret;
}

bool AdjacencyMatrix::fill_board(char** board, std::set<char>& to_add, int letter_idx, int x, int y,
                                 int iter) {
    //if (board[x][y] != 'X') return false;

    // add character at current position
    char curr = index_to_char(letter_idx);
    board[x][y] = curr;
    to_add.erase(curr);
    std::cout << "Curr: " << curr << std::endl;

    // get each neighbor we want this character to have
    std::set<char> neighbors = adj_mat_neighbors(this, curr);
    for (char n : neighbors) {
        std::set<char> neighbor_neighbors = adj_mat_neighbors(this, n);

        std::set<char> intersection;
        std::set_intersection(neighbors.begin(), neighbors.end(),
                              neighbor_neighbors.begin(), neighbor_neighbors.end(),
                              std::inserter(intersection, intersection.begin()));

        std::cout << "Neighboring: " << n << ":   ";
        for (char s : intersection)
            std::cout << s << " ";
        std::cout << std::endl;
    }


    return true;
}

// converts to board (ASSUMPTION: adjacency matrix is valid)
Board* AdjacencyMatrix::to_board() {
    Board* b = new Board(5);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            b->board_state[i][j] = 'X';

    std::set<char> to_add;
    char alphabet[] = "abcdefghiklmnopqrstuvwxyz";
    for (char c : alphabet)
        to_add.insert(c);

    int x = std::rand() % 5;
    int y = std::rand() % 5;
    int letter_idx = std::rand() % 25;
    
    fill_board(b->board_state, to_add, letter_idx, x, y, 0);


    return b;
}
