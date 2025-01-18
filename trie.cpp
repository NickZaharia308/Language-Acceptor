#include "trie.h"

// TrieNode Constructor
TrieNode::TrieNode() {
    for (int i = 0; i < 27; ++i) {
        children[i] = nullptr;
    }
    isEndOfWord = false;
}

// Trie Constructor
Trie::Trie() {
    root = new TrieNode();
}

// Trie Destructor
Trie::~Trie() {}

// Function that converts a character to its index
int Trie::fromCharToIndex(char ch) {
    if (ch == '.') {
        return 26;
    }
    return ch - 'a';
}

// Function that converts an index to its character
char Trie::fromIndexToChar(int index) {
    if (index == 26) {
        return '.';
    }
    return index + 'a';
}

// Function that returns the number of children of a node
int Trie::getNumberOfChildren(TrieNode* root) {
    int count = 0;
    for (int i = 0; i < 27; i++) {
        if (root->children[i] != nullptr) {
            count++;
        }
    }
    return count;
}

// Function that Inserts a word into the trie
void Trie::insert(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int index = fromCharToIndex(ch);
        if (node->children[index] == nullptr) {
            node->children[index] = new TrieNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
}

// Function that searches for a word in the trie
bool Trie::search(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int index = fromCharToIndex(ch);
        if (node->children[index] == nullptr) {
            return false;
        }
        node = node->children[index];
    }

    // True if the node is the end of a valid word
    return node->isEndOfWord;
}

// Function that returns the root of the trie
TrieNode* Trie::getRoot() {
    return root;
}

// Function that prints the regex of the trie
void Trie::printTrieRegex(TrieNode* root, string& regex) {
    if (root == nullptr)
        return;

    int no_of_children = getNumberOfChildren(root);

    // If there are multiple children a parenthesis should be added
    if (no_of_children > 1) {
        regex += "(";
    }

    // We should track if this is the first child to handle "|" appropriately
    bool firstChild = true;

    // Traverse the children
    for (int i = 0; i < 27; i++) {
        if (root->children[i] != nullptr) {
            if (!firstChild) {

                // Adding '|' before processing the next child
                regex += "|";
            }
            firstChild = false;

            // This is the character for the current child
            regex += fromIndexToChar(i);

            // Recurse for the child node
            printTrieRegex(root->children[i], regex);
        }
    }

    // Add closing parenthesis if there were multiple children
    if (no_of_children > 1) {
        regex += ")";
    }
}