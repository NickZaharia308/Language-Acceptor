#include "trie.h"

// TrieNode Constructor
TrieNode::TrieNode() {
    for (int i = 0; i < 26; ++i) {
        // All children should be initialized to nullptr
        children[i] = nullptr;
    }
    // Initially, no word ends at this node
    isEndOfWord = false;
}

// Trie Constructor
Trie::Trie() {
    root = new TrieNode();
}

// Trie Destructor
Trie::~Trie() {}

// Convert a character to its index
int Trie::fromCharToIndex(char ch) {
    return ch - 'a';
}

// Convert an index to its character
char Trie::fromIndexToChar(int index) {
    return index + 'a';
}

// Function that returns the number of children of a node
int Trie::getNumberOfChildren(TrieNode* root) {
    int count = 0;
    for (int i = 0; i < 26; i++) {
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
    // Mark the end of the word
    node->isEndOfWord = true;
}

// Function that searches for a word in the trie
bool Trie::search(const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int index = fromCharToIndex(ch);
        if (node->children[index] == nullptr) {
            // Character not found
            return false;
        }
        node = node->children[index];
    }
    // True if the node is the end of a valid word
    return node->isEndOfWord;
}

// Function checking if there is any word in the trie
// that starts with the given prefix
bool Trie::startsWithPrefix(const string& prefix) {
    TrieNode* node = root;
    for (char ch : prefix) {
        int index = fromCharToIndex(ch);
        if (node->children[index] == nullptr) {
            // Character not found
            return false;
        }
        node = node->children[index];
    }
    // Prefix exists
    return true;
}

// Function that returns the root of the trie
TrieNode* Trie::getRoot() {
    return root;
}

// Function that prints the regex of the trie
void Trie::printTrieRegex(TrieNode* root, string& regex) {
    if (root == nullptr) return;

    int no_of_children = getNumberOfChildren(root);

    // Add opening parenthesis if there are multiple children
    if (no_of_children > 1) {
        regex += "(";
    }

    bool firstChild = true; // Track if this is the first child to handle "|" appropriately

    // Traverse the children
    for (int i = 0; i < 26; i++) {
        if (root->children[i] != nullptr) {
            if (!firstChild) {
                regex += "|"; // Add '|' before processing the next child
            }
            firstChild = false;

            // Add the character for the current child
            regex += fromIndexToChar(i);

            // Recurse for the child node
            printTrieRegex(root->children[i], regex);
        }
    }

    // Add closing parenthesis if there were multiple children
    if (no_of_children > 1) {
        regex += ")";
    }

    // Add an end marker if this node represents the end of a word
    if (root->isEndOfWord) {
        regex += ""; // You can append markers like "$" or leave it as is for standard regex
    }
}