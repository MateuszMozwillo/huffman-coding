#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct TreeNode {
    double propability;
    char coded_char;
    std::string code;

    TreeNode *left;
    TreeNode *right;

    TreeNode(const double p, const char coded_char = ' ', TreeNode *l = nullptr, TreeNode *r = nullptr) {
        this->propability = p;
        this->coded_char = coded_char;
        this->left = l;
        this->right = r;
        this->code = "";
    }

    ~TreeNode() {
        if (this->left != nullptr) {
            delete this->left;
        }
        if (this->right != nullptr) {
            delete this->right;
        }
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
        if (this->left == nullptr && this->right == nullptr) {
            std::cout << "\npropability: " << this->propability << "\ncoded value: " << this->coded_char << "\ncode: " << this->code << "\n";
        }
        if (this->left != nullptr) {
            this->left->print();
        }
        if (this->right != nullptr) {
            this->right->print();
        }
    }

    void extract_leafs(std::vector<TreeNode*>& leafs) {
        if (this->left == nullptr && this->right == nullptr) {
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
        delete this;
        return codes;
    }
};

struct CompareTreeNodes {
    bool operator()(const TreeNode* a, const TreeNode* b) const {
        return a->propability > b->propability;
    }
};

std::string load_text_from_file(const std::string& file_path) {
    std::string line;
    std::string text;
    std::ifstream file(file_path);
    while(getline(file, line)) {
        text += line + "\n";
    }
    file.close();
    return text;
}

std::vector<char> text_to_tokens(const std::string& to_tokenize) {
    std::vector<char> result;
    for (const auto& c : to_tokenize) {
        result.push_back(c);
    }
    return result;
}

TreeNode* create_tree(const std::unordered_map<char, uint64_t>& map, const size_t word_count_sum ) {

    std::priority_queue<TreeNode*, std::vector<TreeNode*>, CompareTreeNodes> q;

    for (auto const& [key, val] : map) {
        q.push(new TreeNode((long double)val/(long double)word_count_sum, key));
    }

    while (q.size() > 1) {
        TreeNode* temp1 = q.top();
        q.pop();
        TreeNode* temp2 = q.top();
        q.pop();
        q.push(new TreeNode(temp1->propability + temp2->propability, ' ', temp1, temp2));
    }

    TreeNode* root = q.top();
    root->encode_nodes();

    return root;
}

std::unordered_map<char, std::string> create_dictionary_from_tokens(const std::vector<char>& chars) {
    std::unordered_map<char, uint64_t> word_occurrence_map;
    for (const auto& c : chars) {

        if (word_occurrence_map.find(c) == word_occurrence_map.end()) {
            word_occurrence_map[c] = 1;
        } else {
            word_occurrence_map[c]++;
        }
    }

    TreeNode* tree_root = create_tree(word_occurrence_map, chars.size());
    std::unordered_map<char, std::string> dict = tree_root->load_codes_from_tree();

    return dict;
}

std::string dictionary_to_string(const std::unordered_map<char, std::string> &dict) {
    std::string map_as_string = "";
    for (auto const& [key, val]: dict) {
        map_as_string += key;
        map_as_string += ": " + val + '\n';
    }
    return map_as_string;
}
