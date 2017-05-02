#include "greedy.h"

Greedy::Greedy(std::vector<std::string> wordlist) {
    this->wordlist = wordlist;

    string_length_compare s;
    std::sort(this->wordlist.begin(), this->wordlist.end(), s);

    adj = new AdjacencyMatrix();
}

Greedy::~Greedy() {

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
    std::vector<std::string> added_words;
    for (int i = 0; i < (int)wordlist.size(); i++) {
        candidate = new AdjacencyMatrix(adj, wordlist[i]);

        if (candidate->is_valid()) {
            delete adj;
            adj = candidate;

            added_words.push_back(wordlist[i]);
        }
    }

    // convert adjacency matrix to board, and print it out
    Board* out_board = candidate->to_board();
    if (out_board != NULL) {
        out_board->print();
    }
    delete out_board;

    // TODO: handle edge case where less than 25 words could be added
    /*
    std::cout << "Longest 25 words on board:" << std::endl;
    for (int i = 0; i < 25; i++)
        std::cout << "\t" << added_words[i] << std::endl;
    */
}


