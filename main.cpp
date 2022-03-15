#include <string>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <future>
#include <chrono>
#include "trie.h"
#define MAX_DEPTH 5

namespace fs = std::experimental::filesystem;
using recursive_directory_iterator = std::experimental::filesystem::recursive_directory_iterator;
using directory_iterator = std::experimental::filesystem::directory_iterator;
using namespace std;

atomic<unsigned int> nthreads{1};
atomic<unsigned long long> searchFiles{0};

// Construct valid extension
const unordered_set<string> validExtensions = {"tiff", "tif", "ndpi", "png", "jpg", "jpeg"};

vector<string> readPrefixListFile(string const& filePath) {
    ifstream infile(filePath);
    vector<string> prefixList;
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            prefixList.push_back(line);
        }
        infile.close();
    }
    return prefixList;
}

void appendPath(const fs::path p, Trie* trie, unordered_map<string, vector<string>>& file2path) {
    const string path_str = p.string();
    const string filename = p.filename();
    const string extension = filename.substr(filename.find_last_of(".") + 1);
    if (validExtensions.find(extension) == validExtensions.end()) return;
    int position = -1;
    bool isExist = trie->hasPrefix(filename, position);
    if (!isExist) return;
    file2path[filename.substr(0, position + 1)].push_back(path_str);
}

unordered_map<string, vector<string>> find_files(fs::path p, Trie* trie) {
    ++nthreads;
    unordered_map<string, vector<string>> file2path;
    
    for (const auto& dirEntry : recursive_directory_iterator(p, fs::directory_options::skip_permission_denied)) {
        const auto p = dirEntry.path();
        if (fs::is_regular_file(p)) {
            ++searchFiles;
            appendPath(p, trie, file2path);
        }
    }
    return file2path;
}

void dfs(string root, int depth, unordered_map<string, vector<string>>& file2path, Trie* trie, vector<future<unordered_map<string, vector<string>>>>& futures) {
    for (const auto& dirEntry : directory_iterator(root, fs::directory_options::skip_permission_denied)) {
        const auto p = dirEntry.path();
        if (fs::is_directory(p)) {
            if (depth == MAX_DEPTH) futures.emplace_back(async(launch::async, find_files, p, trie) ) ;
            else dfs(p.string(), depth + 1, file2path, trie, futures);
        }
        else {
            ++searchFiles;
            appendPath(p, trie, file2path);
        }
    }
}

int main(int argc, char *argv[]) {

    const auto start = chrono::steady_clock::now();

    // Read prefix list
    vector<string> prefixList = readPrefixListFile(argv[1]);

    // Construct trie
    Trie* trie = new Trie();
    
    // Construct output mapping
    unordered_map<string, vector<string>> file2path;
    
    // Insert prefix
    for (auto prefix : prefixList) {
        trie->insert(prefix);
        file2path[prefix] = vector<string>();
    }

    // Recursively find files
    vector<future<unordered_map<string, vector<string>>>> futures;
    dfs(argv[2], 1, file2path, trie, futures);
    
    // Collection
    for (auto& f : futures) {
        auto v = f.get() ;
        for (auto element : v) {
            for (auto path : element.second) file2path[element.first].push_back(path);
        }
    }

    // Output results
    for (auto element : file2path) {
        cout << "[PREFIX] " << element.first << endl;
        if (element.second.size() == 0) cout << "    Not found" << endl;
        else {
            for (auto path : element.second) {
                cout << "    " << path << endl;
            }
        }
        cout << endl;
    }

    // Stat.
    const auto dur = chrono::steady_clock::now() - start;
    cout << "Elapsed wall-clock time " << chrono::duration_cast<chrono::seconds>(dur).count() << " seconds." << endl;
    cout << "#files search: " << searchFiles << endl;
    cout << "#threads: " << nthreads << endl;

    return 0;
}

