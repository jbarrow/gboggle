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
    return (to_board() != NULL);
}


bool AdjacencyMatrix::fill_board(Board* board, std::map<char, std::set<char>>& constraints,
                                 char to_place,
                                 std::set<std::pair<int, int>> possible_positions,
                                 int iter) {
    // if we've already placed this piece, return true
    if (board->has_char(to_place)) return true;

    // for each possible position passed in
    for (std::pair<int, int> pos : possible_positions) {
        bool violation = false;

        int x = pos.first;
        int y = pos.second;

        // make a new board with the piece inserted here
        Board* new_board = new Board(*board);
        if (new_board->board_state[x][y] == 'X') {
            new_board->board_state[x][y] = to_place;
        } else {
            delete new_board;
            continue;
        }

        // get current neighbors, and what neighbors are supposed to be
        std::set<char> curr_neighbors = new_board->chars_neighboring(x, y);
        std::set<char> supposed_neighbors = constraints[to_place];

        // loop through what neighbors are supposed to be. if the neighbor has already been
        // placed, it should be in curr_neighbors (if not, we're in violation of a constraint,
        // and continue to next position). if the neighbor hasn't already been placed, we add it
        // to the list of things to place
        std::set<char> neighbors_to_add;
        for (char neigh : supposed_neighbors) {
            if (new_board->has_char(neigh)) {
                if (curr_neighbors.count(neigh) == 0) {
                    violation = true;
                    break;
                }
            } else {
                neighbors_to_add.insert(neigh);
            }
        }
        if (violation) {
            delete new_board;
            continue;
        }

        // for each neighbor we need to place
        for (char neigh : neighbors_to_add) {
            // get blank spaces around us
            std::set<std::pair<int, int>> blank_spaces =
                new_board->blank_spaces_neighboring(x, y);

            // try to add neighbor in any of these blank spaces
            bool add_success = fill_board(new_board, constraints,
                                          neigh, blank_spaces,
                                          iter + 1);

            // if we can't, we have a violation
            if (!add_success) {
                violation = true;
                break;
            }
        }
        if (violation) {
            delete new_board;
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

// converts to board
Board* AdjacencyMatrix::to_board() {
    // if any of the diagonals of the adjacency matrix are set (i.e. a letter
    // has to occur multiple times in a row), return NULL -- we can't make this
    // board
    for (int i = 0; i < 25; i++)
        if (mat[i][i]) return NULL;

    std::map<char, std::set<char>> constraints = to_map(false);

    // if any letter has more than 8 neighbors, return NULL
    for (auto const& kv_pair : constraints)
        if (kv_pair.second.size() > 8) return NULL;

    Board* b = new Board(5);
    std::set<char> chars_on_board;


    // insert characters that have constraints on them first
    for (auto const& kv_pair : constraints) {
        char c = kv_pair.first;

        chars_on_board = b->chars();
        if (chars_on_board.count(c) == 0) {
            std::set<std::pair<int, int>> blanks =
                b->blank_spaces();

            bool success = fill_board(b, constraints, c, blanks, 0);
            if (!success) {
                delete b;
                return NULL;
            }
        }
    }

    // insert remaining characters in blank spaces
    chars_on_board = b->chars();
    for (char c : "abcdefghiklmnopqrstuvwxyz") {
        if (chars_on_board.count(c) == 0) {
            bool added = false;

            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (b->board_state[i][j] == 'X') {
                        b->board_state[i][j] = c;
                        added = true;
                        break;
                    }
                }
                if (added) break;
            }
        }
    }

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

        std::set<char> neighbors;

        bool* adj_neighbors = mat[i];
        for (int j = 0; j < 25; j++) {
            if (adj_neighbors[j]) {
                neighbors.insert(index_to_char(j));
            }
        }

        if (include_empty || !neighbors.empty())
            ret[curr] = neighbors;
    }

    return ret;
}


