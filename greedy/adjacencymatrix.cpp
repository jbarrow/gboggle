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

std::set<std::pair<int, int>> blank_neighbors(char** board, int x, int y) {
    std::set<std::pair<int, int>> ret;

    for (int x_off = -1; x_off <= 1; x_off++) {
        for (int y_off = -1; y_off <= 1; y_off++) {
            if (x_off == 0 && y_off == 0) continue;

            int x_pos = x + x_off;
            int y_pos = y + y_off;

            if (x_pos < 0) x_pos += 5;
            if (x_pos > 4) x_pos -= 5;
            if (y_pos < 0) y_pos += 5;
            if (y_pos > 4) y_pos -= 5;

            if (board[x_pos][y_pos] == 'X')
                ret.insert(std::make_pair(x_pos, y_pos));
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

bool AdjacencyMatrix::fill_board(Board* board, std::map<char, std::set<char>>& constraints,
                                 std::set<char>& placed, char to_place,
                                 std::set<std::pair<int, int>> possible_positions,
                                 int iter) {
    // if character has already been placed, return true (?)
    if (placed.count(to_place) > 0) return true;


    // for each possible position passed in
    for (std::pair<int, int> pos : possible_positions) {
        bool violation = false;

        int x = pos.first;
        int y = pos.second;

        // make a new board with the piece inserted here
        Board* new_board = new Board(*board);
        new_board->board_state[x][y] = to_place;
        placed.insert(to_place);

        // get current neighbors, and what neighbors are supposed to be
        std::set<char> curr_neighbors = board_neighbors(new_board->board_state, x, y);
        std::set<char> supposed_neighbors = constraints[to_place];

        // loop through what neighbors are supposed to be. if the neighbor has already been
        // placed, it should be in curr_neighbors (if not, we're in violation of a constraint,
        // and continue to next position). if the neighbor hasn't already been placed, we add it
        // to the list of things to place
        std::set<char> neighbors_to_add;
        for (char neigh : supposed_neighbors) {
            if (placed.count(neigh) > 0) {
                if (curr_neighbors.count(neigh) == 0) {
                    violation = true;
                    break;
                }
            } else {
                neighbors_to_add.insert(neigh);
            }
        }
        if (violation) {
            placed.erase(to_place);
            continue;
        }

        // for each neighbor we need to place
        int neighbors_added = 0;
        for (char neigh : neighbors_to_add) {
            // get blank spaces around us
            std::set<std::pair<int, int>> blank_spaces =
                blank_neighbors(new_board->board_state, x, y);

            // if there are more neighbors to add than blanks, that's a violation
            if ((int)neighbors_to_add.size() - neighbors_added > (int)blank_spaces.size()) {
                violation = true;
                break;
            }

            bool add_success = fill_board(new_board, constraints,
                                          placed, neigh, blank_spaces,
                                          iter + 1);
            if (!add_success) {
                violation = true;
                break;
            }

            neighbors_added++;
        }
        if (violation) {
            placed.erase(to_place);
            continue;
        }

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                board->board_state[i][j] = new_board->board_state[i][j];

        delete new_board;
        return true;
    }

    // if we weren't able to place the character at any of the possible positions,
    // return false
    return false;
}

// converts to board (ASSUMPTION: adjacency matrix is valid)
Board* AdjacencyMatrix::to_board() {
    std::map<char, std::set<char>> constraints = to_map(false);

    std::set<char> alphabet;
    for (char c : "abcdefghiklmnopqrstuvwxyz")
        alphabet.insert(c);

    std::set<char> placed;

    Board* b = new Board(5);
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            b->board_state[i][j] = 'X';

    std::set<std::pair<int, int>> initial_positions;
    initial_positions.insert(std::make_pair(0, 0));

    bool success = fill_board(b, constraints, placed, 's', initial_positions, 0);
    if (!success)
        std::cout << "!!! FAILED !!!" << std::endl;

    return b;
}

void AdjacencyMatrix::print() {
    std::map<char, std::set<char>> adj_map = to_map(false);

    for (auto iter = adj_map.begin(); iter != adj_map.end(); iter++) {
        char curr = iter->first;
        std::set<char> neighbors = iter->second;

        std::cout << curr << ": ";
        for (char c : neighbors)
            std::cout << c << " ";
        std::cout << std::endl;
    }
}

std::map<char, std::set<char>> AdjacencyMatrix::to_map(bool include_empty) {
    std::map<char, std::set<char>> ret;

    for (int i = 0; i < 25; i++) {
        char curr = index_to_char(i);

        std::set<char> neighbors = adj_mat_neighbors(this, curr);
        if (include_empty || !neighbors.empty())
            ret[curr] = neighbors;
    }

    return ret;
}


