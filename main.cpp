#include <iostream>
#include <string>
#include <fstream>
#include "trie.h"

using namespace std;

int main() {
    int no_of_accepted_words, no_of_rejected_words;
    int word_length;
    string regex;

    // Creating the input and the output file
    ifstream in_file("input.txt");
    ofstream out_file("output.txt");

    // Reading the first line of the file
    in_file >> no_of_accepted_words >> no_of_rejected_words >> word_length;

    // Mark the output file as a regex file (not a DFA file)
    out_file << "regex" << endl;

    // Creating a trie to store the accepted words
    Trie trie;
    for (int i = 0; i < no_of_accepted_words; ++i) {
        string word;
        in_file >> word;
        trie.insert(word);
    }

    trie.printTrieRegex(trie.getRoot(), regex);
    out_file << regex << endl;

    return 0;
}