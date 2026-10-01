#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "huffman.hpp"

struct TreeNode {
    uint64_t count;
    char coded_char;
    std::string code;

    TreeNode *left;
    TreeNode *right;

    TreeNode(const uint64_t count, const char coded_char = ' ', TreeNode *l = nullptr, TreeNode *r = nullptr) {
        this->count = count;
        this->coded_char = coded_char;
        this->left = l;
        this->right = r;
        this->code = "";
    }

    ~TreeNode() {
        delete this->left;
        delete this->right;
    }

    bool is_leaf() const {
        return this->left == nullptr && this->right == nullptr;
    }

    void encode_nodes(const std::string& current_code = "") {
        this->code = current_code;
        if (this->left != nullptr) {
            this->left->encode_nodes(this->code + "0");
        }
        if (this->right != nullptr) {
            this->right->encode_nodes(this->code + "1");
        }
    }

    void print() {
        if (this->is_leaf()) {
            std::cout << "\ncount: " << this->count << "\ncoded value: " << this->coded_char << "\ncode: " << this->code << "\n";
        }
        if (this->left != nullptr) {
            this->left->print();
        }
        if (this->right != nullptr) {
            this->right->print();
        }
    }

    void extract_leafs(std::vector<TreeNode*>& leafs) {
        if (this->is_leaf()) {
            leafs.push_back(this);
        }
        if (this->left != nullptr) {
            this->left->extract_leafs(leafs);
        }
        if (this->right != nullptr) {
            this->right->extract_leafs(leafs);
        }
    }

    std::unordered_map<char, std::string> load_codes_from_tree() {
        std::unordered_map<char, std::string> codes;
        std::vector<TreeNode*> leafs;
        this->extract_leafs(leafs);
        for (auto const& leaf : leafs) {
            codes[leaf->coded_char] = leaf->code;
        }
        return codes;
    }
};

struct CompareTreeNodes {
    bool operator()(const TreeNode* a, const TreeNode* b) const {
        return a->count > b->count;
    }
};

std::string load_text_from_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file: " + file_path);
    }
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

std::vector<char> text_to_tokens(const std::string& to_tokenize) {
    return std::vector<char>(to_tokenize.begin(), to_tokenize.end());
}

static TreeNode* create_tree(const std::unordered_map<char, uint64_t>& map) {
    if (map.empty()) {
        return nullptr;
    }

    std::priority_queue<TreeNode*, std::vector<TreeNode*>, CompareTreeNodes> q;

    for (auto const& [key, val] : map) {
        q.push(new TreeNode(val, key));
    }

    while (q.size() > 1) {
        TreeNode* temp1 = q.top();
        q.pop();
        TreeNode* temp2 = q.top();
        q.pop();
        q.push(new TreeNode(temp1->count + temp2->count, ' ', temp1, temp2));
    }

    TreeNode* root = q.top();
    // a single distinct symbol still needs a non-empty code
    root->encode_nodes(root->is_leaf() ? "0" : "");

    return root;
}

std::unordered_map<char, std::string> create_dictionary_from_tokens(const std::vector<char>& chars) {
    std::unordered_map<char, uint64_t> char_occurrence_map;
    for (const auto& c : chars) {
        char_occurrence_map[c]++;
    }

    TreeNode* tree_root = create_tree(char_occurrence_map);
    if (tree_root == nullptr) {
        return {};
    }
    std::unordered_map<char, std::string> dict = tree_root->load_codes_from_tree();
    delete tree_root;

    return dict;
}

// one entry per line: "<byte value 0-255> <code>", so '\n' or ':' in the input can't break the header
std::string dictionary_to_string(const std::unordered_map<char, std::string> &dict) {
    std::string map_as_string = "";
    for (auto const& [key, val]: dict) {
        map_as_string += std::to_string(static_cast<unsigned char>(key));
        map_as_string += ' ' + val + '\n';
    }
    return map_as_string;
}

std::string encode_tokens(const std::vector<char>& tokens, const std::unordered_map<char, std::string>& dict, int& padded_bits) {
    std::string bytes;
    unsigned char current = 0;
    int bit_count = 0;

    for (const auto& token : tokens) {
        for (const char bit : dict.at(token)) {
            current = static_cast<unsigned char>((current << 1) | (bit == '1'));
            if (++bit_count == 8) {
                bytes.push_back(static_cast<char>(current));
                current = 0;
                bit_count = 0;
            }
        }
    }

    padded_bits = 0;
    if (bit_count > 0) {
        padded_bits = 8 - bit_count;
        bytes.push_back(static_cast<char>(current << padded_bits));
    }
    return bytes;
}
