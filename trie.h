#ifndef _TRIE_H_
#define _TRIE_H_

#include <vector>
#include <string>
using namespace std;

class TrieNode {
public:
    bool isEnd;
    vector<TrieNode*> children;
    TrieNode();
};

class Trie {
public:
    TrieNode* root;
    Trie();
    void insert(string const& word);
    bool search(string const& word);
    bool startsWith(string const& prefix);
    bool hasPrefix(string const& prefix, int& position);
};

#endif