#include <vector>
#include <string>
#include "trie.h"
using namespace std;

TrieNode::TrieNode() {
    this->isEnd = false;
    this->children = vector<TrieNode*>(256, nullptr);
}

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(string const& word) {
    TrieNode* curr = root;
    for (auto c : word){
        if (curr->children[c] == nullptr){
            curr->children[c] = new TrieNode();
        }
        curr = curr->children[c];
    }
    curr->isEnd = true;
}
bool Trie::search(string const& word) {
    TrieNode* curr = root;
    for (auto c : word){
        if (curr->children[c] == nullptr) return false;
        curr = curr->children[c];
    }
    return curr != nullptr && curr->isEnd;
    
}
bool Trie::startsWith(string const& prefix) {
        TrieNode* curr = root;
        for (auto c : prefix){
            if (curr->children[c] == nullptr) return false;
            curr = curr->children[c];
        }
        return curr != nullptr;
    }

bool Trie::hasPrefix(string const& prefix, int& position) {
    TrieNode* curr = root;
    for (int i = 0; i < prefix.size(); i++) {
        if (curr->isEnd) return true;
        if (curr->children[prefix[i]] == nullptr) return false;
        curr = curr->children[prefix[i]];
        position = i;
    }
    return curr != nullptr && curr->isEnd;
}
