# Language Acceptor

## Copyright 2025 Zaharia Nicusor-Alexandru

### Project Idea

The project creates a regex (of minimal size) that matches a given number of
accepted words and rejects a given number of rejected words.

### How it works

1. After reading all the words (both accepted and rejected), a scan to check if
all accepted words have a **common letter** on a **specific position**.
Using this, some rejected words may be fastly discarded (if they don't have
the specific character on the specific position).
If after filtering the rejected words there are no more rejected words, the
regex is found.
Otherwise the program goes to the next step.
2. A function to find all unique positions is called.
Unique positions are consecutive indexes in a vector that represent the
smallest portion of a word that do not match any other rejected words.
Basically, the substrings of the accepted words between those consecutive
indexes don't match any rejected word.
Thus, a simple '|' (or) between these portions will match all accepted words
and won't accept the rejected words.
3. An additional improvement to that is finding unique characters of accepted
words.
A unique character is a character in a position that no other rejected word has
in the same position (other accepted words may have it).
4. By combining these two ideas and trying both a regex using a **trie**
implementation and a '|' between unique positions a minimal regex is created
that is written in an output file.
