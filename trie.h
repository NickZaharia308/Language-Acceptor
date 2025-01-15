#include <string>
#include <fstream>

using namespace std;

// Inspired from GeeksForGeeks: https://www.geeksforgeeks.org/trie-insert-and-search/

// The structure for a node in the trie
struct TrieNode {
    // Fixed array for 26 lowercase letters of english alphabet
    TrieNode* children[26];
    
    // This is true only if the node marks the end of a word
    bool isEndOfWord;

    TrieNode();
};

// Trie class
class Trie {
private:
    // Root for my trie
    TrieNode* root;

    // Useful function to convert char to array index for the children array
    int fromCharToIndex(char ch);
    char fromIndexToChar(int index);
    int getNumberOfChildren(TrieNode* root);

public:
    Trie();
    ~Trie();

    void insert(const string& word);
    bool search(const string& word);
    bool startsWithPrefix(const string& prefix);
    TrieNode* getRoot();
    void printTrieRegex(TrieNode* root, string &regex);
};
