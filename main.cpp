#include <iostream>
#include <string>
#include <fstream>

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

    // Reading the accepted words
    for (int i = 0; i < no_of_accepted_words; i++) {
        in_file >> regex;

        // If the word is not the last word, add a pipe (or) at the end
        // Else, just the word (for the last word)
        if (i != no_of_accepted_words - 1) {
            out_file << regex << "|";
        } else {
            out_file << regex;
        }
    }

    return 0;
}