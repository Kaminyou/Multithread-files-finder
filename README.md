# Multithread files finder
### Enjoy searching millions of files in few minutes!
Given a prefix list, find files match to each prefix with Trie (multithreading). Matching for all prefixes in prefix list for one file name is in O(L), where L denotes the # of chars of the file name.

## Usage
### Preparation
1. Please modify `prefix_list` to specify all the prefixes that should be matched.

2. Please modify `MAX_DEPTH` and `validExtensions` in `main.cpp`. The default settings are:
```cpp
#define MAX_DEPTH 5
const unordered_set<string> validExtensions = {"tiff", "tif", "ndpi", "png", "jpg", "jpeg"};
```
- **MAX_DEPTH:** The maximum depth of recursion from the root path (single thread). If recursive depth is over this threshold, a subprocess to recursively search the following hierachy will be triggered (multi-thread).
- **validExtensions:** To specify the extensions which should be considered.
### Compile
```
$ make
```
### Uncomment stat. info (option)
As atomic operations would make the process slower, they are commented out by default. You can uncomment them in `main.cpp` if more stat. info is required.
```cpp
// Please uncomment the following two lines to get more stat. info
// atomic<unsigned int> nthreads{1};
// atomic<unsigned long long> searchFiles{0};
```
```cpp
// Please uncomment the following two lines to get more stat. info
// cout << "#files search: " << searchFiles << endl;
// cout << "#threads: " << nthreads << endl;
```

### Execute
It is recommended to run this program with **root privileges** to search all the files bypassing permission issues.
```script
$ ./main [PREFIX_LIST_PATH] [ROOT_PATH]
```
e.g.
```
$ ./main prefix_list.txt /home/
```
### Output example
```script
[PREFIX] config
    Not found

[PREFIX] profile
    /home/user32/aaa/bbb/profile_3891.jpg

[PREFIX] bb67ae85
    /home/user1/zzz/kkk/bb67ae85ef-we.tiff
    /home/user2/eee/fff/bb67ae8531cs-q.tiff

Elapsed wall-clock time 3 seconds.
#files search: 49031
#threads: 227
```
## Bug shooting
If the following error is encountered, please decrease **`MAX_DEPTH`**. `ulimit -a` can be used to check the maximum thread you can use.
```
terminate called after throwing an instance of ‘std::system_error’ What(): Resource temporarily unavaible
```
## Note
You **should not** provide any prefix which is the prefix of another prefix. For example, a prefix `ABCD` would be compromised if a prefix `ABC` is also provided. 