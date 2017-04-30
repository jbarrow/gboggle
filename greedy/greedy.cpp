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
    std::cout << "Top 10 longest words:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "\t" << wordlist[i] << std::endl;
    }

    std::vector<std::string> added_words;

    AdjacencyMatrix* candidate;
    for (int i = 0; i < (int)wordlist.size(); i++) {
        candidate = new AdjacencyMatrix(adj, wordlist[i]);

        if (candidate->is_valid()) {
            delete adj;
            adj = candidate;

            added_words.push_back(wordlist[i]);
        }
    }

    std::cout << "Longest 25 words on board:" << std::endl;
    for (int i = 0; i < 25; i++)
        std::cout << "\t" << added_words[i] << std::endl;
}


