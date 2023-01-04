#include <unordered_map>
#include <string>
#include <iostream>
#include <ranges>
#include <numeric>
#include <vector>
#include <memory>
#include <algorithm>
#include <fstream>


using namespace std;


// Local helpers, since they are not exported, they have local linkage.
//constexpr inline auto values = views::transform([](auto &p) { return p.second; });
auto values_files = [](const size_t previous, const pair<const string, size_t>& p)
                                          { return previous + p.second; };



void skipws(istream& s) {
    while (isspace(s.peek())) s.get();
}

namespace day07::terminal {

struct Directory {
    // Recursive size of the directory.
    size_t recursive_size();
    // Parse content of directory from "$ ls".
    friend istream& operator>>(istream& s, Directory& dir);

    // Required for "$ cd ..".
    Directory *parent = nullptr;
    // Required for "$ cd subdirectory".
    unordered_map<string, Directory*> subdirs;
    // Information about files.
    unordered_map<string, size_t> files;
private:
    // Cache, so we do not repeatedly re-calculate the size.
    size_t recursive_size_;
};

struct Tree {
    // Implementation of "$ cd subdirectory", also constructs the directory.
    Directory* cd(Directory* current, string dir);
    // Directory root.
    Directory root;
    // Storage for all Directories.
    vector<unique_ptr<Directory>> storage;
};

auto values_dirs = [](const size_t previous, const pair<const string, Directory*>& p)
                                          { return previous + p.second->recursive_size(); };

size_t Directory::recursive_size() {
    // If we have a cached value, we can simply return it.
    if (recursive_size_) return recursive_size_;

    // Normally we would use views::values to get pair::second,
    // but libc++ has not implemented this view yet.

    // Sizes of files in this directory.
    //auto fsz = files | values | views::common;

    auto fsz = accumulate(begin(files), end(files), 0, values_files);
    // Sizes of subdirectories in this directory.
    //auto dsz = subdirs | values | views::transform(&Directory::recursive_size) | views::common;
    auto dsz = accumulate(begin(subdirs), end(subdirs), 0, values_dirs);

    // Sum up the sizes and return.
    //recursive_size_ = reduce(fsz.begin(), fsz.end()) + reduce(dsz.begin(), dsz.end());
    recursive_size_ = fsz + dsz;
    return recursive_size_;
}

istream& operator>>(istream& s, Directory& dir) {
    // Read until the next "$ ls" or "$ cd" or end of input
    while (s.peek() != '$' && s) {
        if (isdigit(s.peek())) {
            // Parse a file-size and store.
            size_t file_size;
            string filename;
            s >> file_size >> filename;
            dir.files[filename] = file_size;
            skipws(s); // Skip the trailing whitespace.
        } else {
            // Since we handle directory as we enter them using cd, we can ignore them in the ls output.
            string line;
            getline(s, line);
        }
    }
    return s;
}

Directory* Tree::cd(Directory* current, string dir) {
    if (dir == "/") return &root;
    if (dir == "..") return current->parent;

    // Directory already exists.
    if (current->subdirs[dir] != nullptr)
        return current->subdirs[dir];

    // Create and set-up the directory.
    auto &sub_dir = storage.emplace_back(make_unique<Directory>());
    sub_dir->parent = current;
    current->subdirs[dir] = sub_dir.get();
    return current->subdirs[dir];
}

unique_ptr<Tree> parse_output(istream& s) {
    auto tree = make_unique<Tree>();
    Directory *current = &tree->root;
    current->parent = current;

    // Read each command:
    while (s.peek() == '$') {
        char c;
        string cmd;
        // read the '$' token and the command
        s >> c >> cmd;
        if (cmd == "cd") {
            string where_to;
            s >> where_to;
            current = tree->cd(current, where_to);
            skipws(s);
        } else if (cmd == "ls") {
            s >> *current;
        }
    }
    return tree;
}

size_t visit_and_sum_up(Directory *current) {
    // Visit sub-directories and sum up.
    //auto dsz = current->subdirs | values | views::transform(visit_and_sum_up) | views::common;
    //size_t sum = reduce(dsz.begin(), dsz.end());
    size_t sum = 0;
    for (auto elt : current->subdirs)
    {
        sum += visit_and_sum_up(elt.second);
    }
    // If this directory is less than 100000, also add it to the sum:
    if (current->recursive_size() <= 100000)
        sum += current->recursive_size();
    return sum;
}

size_t find_smallest_but_sufficient(Directory *current, size_t goal) {
    // Visit sub-directories, and filter out directories that are not large enough.
    /*
    auto dsz = current->subdirs | values | views::transform([goal](Directory *current) {
        return find_smallest_but_sufficient(current, goal);
    }) | views::filter([goal](size_t value) { return value >= goal; });
    */
    vector<size_t> v_dsz;
    for (auto elt: current->subdirs)
    {
        size_t dsz = find_smallest_but_sufficient(elt.second, goal);
        if (dsz >= goal)
        {
            v_dsz.push_back(dsz);
        }
    }

    // If none of the subdirectories is large enough return the size of this directory.
    if (v_dsz.empty()) return current->recursive_size();
    // Otherwise, return the smallest subdirectory (large enough).
    return *min_element(v_dsz.begin(), v_dsz.end());
}

}