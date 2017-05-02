#include "greedy.h"

Greedy::Greedy(std::vector<std::string> wordlist) {
    this->wordlist = wordlist;

    string_length_compare s;
    std::sort(this->wordlist.begin(), this->wordlist.end(), s);

    adj = new AdjacencyMatrix();
}

Greedy::~Greedy() {
    delete adj;
}

void Greedy::search() {
    // sanity check: print 10 longest words
    std::cout << "Top 10 longest words:" << std::endl;
    for (int i = 0; i < 10; i++) {
        if (i >= (int)wordlist.size()) break;
        std::cout << "\t" << wordlist[i] << std::endl;
    }

    // generate board by repeatedly adding words to adjacency matrix, in order
    // from longest to shortest
    AdjacencyMatrix* candidate;
    Board* out_board = NULL;
    std::vector<std::string> added_words;
    for (int i = 0; i < (int)wordlist.size(); i++) {
        candidate = new AdjacencyMatrix(adj, wordlist[i]);

        Board* candidate_board = candidate->to_board();
        if (candidate_board != NULL) {
            delete adj;
            adj = candidate;

            if (out_board != NULL) delete out_board;
            out_board = candidate_board;

            added_words.push_back(wordlist[i]);
            std::cout << "\r" << i << ": Added " << wordlist[i] << "                         " << std::endl;
        } else {
            delete candidate_board;
            std::cout << "\r" << i << ": Couldn't add " << wordlist[i] << "                  " << std::flush;
        }

        if (added_words.size() >= 20) break;
    }
    std::cout << std::endl;

    // convert adjacency matrix to board, and print it out
    if (out_board != NULL) {
        out_board->print();
        delete out_board;
    } else {
        std::cout << "!!! FAILURE !!!" << std::endl;
    }
}


