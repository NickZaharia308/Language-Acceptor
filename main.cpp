#include <iostream>
#include <string>
#include <fstream>
#include "trie.h"
#include <vector>
#include <cstring>
#include <set>

using namespace std;

// Function to find unique positions with substrings of increasing lengths
vector<int> find_unique_positions(const vector<string>& accepted_words, const vector<string>& rejected_words, int word_length) {
    vector<int> unique_positions;

    for (int length = 2; length <= word_length; length++) {
        for (int pos = 0; pos <= word_length - length; pos++) {
            bool is_unique = true;

            for (const auto& accepted_word : accepted_words) {
                string accepted_substr = accepted_word.substr(pos, length);

                for (const auto& rejected_word : rejected_words) {
                    string rejected_substr = rejected_word.substr(pos, length);

                    if (accepted_substr == rejected_substr) {
                        is_unique = false;
                        break;
                    }
                }

                if (!is_unique) {
                    break;
                }
            }

            // Push the indices of the unique positions
            if (is_unique) {
                // All positions
                for (int i = pos; i < pos + length; i++) {
                    unique_positions.push_back(i);
                }
                return unique_positions;
            }
        }
    }

    return unique_positions;
}

string create_regex(vector<int> unique_positions, char unique_letters[101],
                    int word_length, char accepted_words[401][101],
                    int no_of_accepted_words) {
    string regex = "";
    for (int i = 0; i < word_length; i++) {
        if (unique_positions[0] != i) {
            if (unique_letters[i] != '.') {
                regex += unique_letters[i];
            } else {
                regex += ".";
            }
        } else {
             regex += "(";

            set<string> substrings_set;
            for (int j = 0; j < no_of_accepted_words; j++) {
                string substring = "";
                for (long unsigned int k = 0; k < unique_positions.size(); k++) {
                    substring += accepted_words[j][unique_positions[k]];
                }
                substrings_set.insert(substring);
            }

            vector<string> substrings(substrings_set.begin(), substrings_set.end());
            for (long unsigned int j = 0; j < substrings.size(); j++) {
                regex += substrings[j];
                if (j != substrings.size() - 1) {
                    regex += "|";
                }
            }
            regex += ")";
            i += unique_positions.size() - 1;
        }
    }
    return regex;
}

int main() {
    int no_of_accepted_words, no_of_rejected_words;
    int word_length;
    string regex;
    char accepted_words[401][101];
    vector <string> rejected_words;
    char unique_letters[101];

    // Creating the input and the output file
    ifstream in_file("input.txt");
    ofstream out_file("output.txt");

    // Reading the first line of the file
    in_file >> no_of_accepted_words >> no_of_rejected_words >> word_length;

    // Mark the output file as a regex file (not a DFA file)
    out_file << "regex" << endl;

    // Creating a trie to store the accepted words
    Trie trie;
    for (int i = 0; i < no_of_accepted_words; i++) {
        in_file >> accepted_words[i];
        trie.insert(accepted_words[i]);
    }

    // Reading the rejected words
    for (int i = 0; i < no_of_rejected_words; i++) {
        string rejected_word;
        in_file >> rejected_word;
        rejected_words.push_back(rejected_word);
    }

    // trie.printTrieRegex(trie.getRoot(), regex);
    // out_file << regex << endl;

    // For all accepted words, we should check if a character is common on a
    // specific position in all the words
    for (int i = 0; i < word_length; i++) {
        bool is_common = true;
        for (int j = 0; j < no_of_accepted_words - 1; j++) {
            if (accepted_words[j][i] != accepted_words[j + 1][i]) {
                is_common = false;
                break;
            }
        }
        if (is_common) {
            unique_letters[i] = accepted_words[0][i];
        } else {
            unique_letters[i] = '.';
        }
    }

    // For all rejected words, we should check if they have the unique letters
    // in the specific positions. If they don't, they should be removed from
    // rejected_words, because we want a regex to match the accepted_words and
    // reject the rejected_words in as little as possible number of characters
    // Basically, we should filter out rejected words that don't
    // match the unique letters
    for (auto it = rejected_words.begin(); it != rejected_words.end(); ) {
        bool matches = true;
        for (int i = 0; i < word_length; i++) {
            if (unique_letters[i] != '.' && (*it)[i] != unique_letters[i]) {
                matches = false;
                break;
            }
        }

        if (!matches) {
            // Remove the word if it doesn't match the unique letters
            it = rejected_words.erase(it);
            no_of_rejected_words--;
        } else {
            ++it;
        }
    }

    // cout << unique_letters << endl;

    // // print rejected words
    // for (int i = 0; i < no_of_rejected_words; i++) {
    //     cout << rejected_words[i] << endl;
    // }

    // If there are no more rejected words, we can just return the unique letters
    if (no_of_rejected_words == 0) {
        out_file << unique_letters << endl;
        in_file.close();
        out_file.close();
        return 0;
    }

    // Find the unique positions
    vector<int> unique_positions = find_unique_positions(vector<string>(accepted_words, accepted_words + no_of_accepted_words), rejected_words, word_length);

    // // Print the unique positions
    // for (int i = 0; i < unique_positions.size(); i++) {
    //     cout << unique_positions[i] << endl;
    // }

    // // Create the regex
    string final_regex = create_regex(unique_positions, unique_letters, word_length, accepted_words, no_of_accepted_words);
    // cout << final_regex << endl;

    // Write the regex to the output file
    out_file << final_regex << endl;

    // Closing the files
    in_file.close();
    out_file.close();

    return 0;
}