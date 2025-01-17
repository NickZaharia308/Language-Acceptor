#include <iostream>
#include <string>
#include <fstream>
#include "trie.h"
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>

using namespace std;

string final_regex = string(1000, '.');

vector<vector<int>> unique_chars_for_each_accepted_word = {};

// Function to see if a word is accepted by the regex
// Function to check if a word is accepted by the regex
bool is_accepted_by_regex(const string& word, const string& regex) {
    long unsigned int i = 0, j = 0;
    while (i < word.length() && j < regex.length()) {
        if (regex[j] == '.') {
            i++;
            j++;
        } else if (regex[j] == '(') {
            j++;
            int start = j;
            while (regex[j] != ')') {
                j++;
            }
            string options = regex.substr(start, j - start);
            bool matched = false;
            size_t pos = 0;
            while ((pos = options.find('|')) != string::npos) {
                string option = options.substr(0, pos);
                if (word.substr(i, option.length()) == option) {
                    matched = true;
                    i += option.length();
                    break;
                }
                options.erase(0, pos + 1);
            }
            if (!matched && word.substr(i, options.length()) == options) {
                matched = true;
                i += options.length();
            }
            if (!matched) {
                return false;
            }
            j++;
        } else {
            if (word[i] != regex[j]) {
                return false;
            }
            i++;
            j++;
        }
    }
    return i == word.length() && j == regex.length();
}

// Function to find the unique characters for each accepted word
// Unique means that the character isn't in another rejected word
// on the same position (it can be on another position)
void find_unique_chars_for_each_accepted_word(const vector<string>& accepted_words, const vector<string>& rejected_words, int word_length) {
    for (const auto& accepted_word : accepted_words) {
        vector<int> unique_chars;
        for (int i = 0; i < word_length; i++) {
            bool is_unique = true;
            for (const auto& rejected_word : rejected_words) {
                if (accepted_word[i] == rejected_word[i]) {
                    is_unique = false;
                    break;
                }
            }

            if (is_unique) {
                unique_chars.push_back(i);
            }
        }
        if (unique_chars.size() == 0) {
            unique_chars.push_back(-1);
        }   

        unique_chars_for_each_accepted_word.push_back(unique_chars);
    }
}


// Function to find unique positions with substrings of increasing lengths
vector<int> find_unique_positions(const vector<string>& accepted_words, const vector<string>& rejected_words, int word_length) {
    vector<int> unique_positions;

    for (int length = 1; length <= word_length; length++) {
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
        if (unique_positions.size() >= 1 && unique_positions[0] != i) {
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
                bool start_writing_unique = false;
                bool contains_unique_in_range = false;
                for (long unsigned int k = 0; k < unique_positions.size(); k++) {
                    // If the position of the character is the same as the unique position
                    // then we add the character to the substring, else it will be a dot
                    if (std::find(unique_chars_for_each_accepted_word[j].begin(),
                        unique_chars_for_each_accepted_word[j].end(),
                        unique_positions[k]) != unique_chars_for_each_accepted_word[j].end()) {
                        contains_unique_in_range = true;
                    }
                }

                for (long unsigned int k = 0; k < unique_positions.size(); k++) {
                    // If the position of the character is the same as the unique position
                    // then we add the character to the substring, else it will be a dot
                    if (std::find(unique_chars_for_each_accepted_word[j].begin(),
                        unique_chars_for_each_accepted_word[j].end(),
                        unique_positions[k]) != unique_chars_for_each_accepted_word[j].end()
                        || unique_chars_for_each_accepted_word[j][0] == -1
                        || !contains_unique_in_range) {

                        //cout << accepted_words[j][unique_positions[k]]<< endl;
                        substring += accepted_words[j][unique_positions[k]];
                        start_writing_unique = true;
                    } else {
                        substring += ".";
                    }
                }
               // cout << substring << endl;

                // If the substring contains only dots then we don't add it
                if (substring.find_first_not_of('.') != string::npos) {
                    substrings_set.insert(substring);
                }
            }

            // Insert the substrings into a trie
            Trie trie;
            for (const auto& substring : substrings_set) {
                trie.insert(substring);
            }

            string localTrieRegex;
            trie.printTrieRegex(trie.getRoot(), localTrieRegex);

            //cout << localTrieRegex << endl;

            string localSubSetRegex = "";
            vector<string> substrings(substrings_set.begin(), substrings_set.end());
            for (long unsigned int j = 0; j < substrings.size(); j++) {
                localSubSetRegex += substrings[j];
                if (j != substrings.size() - 1) {
                    localSubSetRegex += "|";
                }
            }

            if (localTrieRegex.size() < localSubSetRegex.size()) {
                regex += localTrieRegex;
            } else {
                regex += localSubSetRegex;
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

    find_unique_chars_for_each_accepted_word(vector<string>(accepted_words, accepted_words + no_of_accepted_words), rejected_words, word_length);

    // Print the accepted words and thei unique characters    
    // for (int i = 0; i < no_of_accepted_words; i++) {
    //     cout << accepted_words[i] << ": ";
    //     for (int j = 0; j < unique_chars_for_each_accepted_word[i].size(); j++) {
    //         cout << unique_chars_for_each_accepted_word[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // // Create the regex
    final_regex = create_regex(unique_positions, unique_letters, word_length, accepted_words, no_of_accepted_words);
    // cout << final_regex << endl;

    // Write the regex to the output file
    out_file << final_regex << endl;
    
    // Closing the files
    in_file.close();
    out_file.close();

    return 0;
}