#include <complex>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

using namespace std;

struct TreeNode {
    double propability;
    string coded_word;
    string code;

    TreeNode *left;
    TreeNode *right;

    TreeNode(double p, string coded_word, TreeNode *l = nullptr, TreeNode *r = nullptr) {
        this->propability = p;
        this->coded_word = coded_word;
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

    void encode_nodes(const string& current_code = "") {
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
            cout << "\npropability: " << this->propability << "\ncoded value: " << this->coded_word << "\ncode: " << this->code << "\n";
        }
        if (this->left != nullptr) {
            this->left->print();
        }
        if (this->right != nullptr) {
            this->right->print();
        }
    }

    void extract_leafs(vector<TreeNode*>& leafs) {
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
};

struct CompareTreeNodes {
    bool operator()(const TreeNode* a, const TreeNode* b) const {
        return a->propability > b->propability;
    }
};

string load_text_from_file(const string& file_path) {
    string line;
    string text;
    ifstream file(file_path);
    while(getline(file, line)) {
        text += line + "\n";
    }
    file.close();
    return text;
}

vector<string> text_to_tokens(const string& to_tokenize) {
    vector<string> result;

    stringstream line_stream(to_tokenize);

    string line;

    while (getline(line_stream, line, '\n')) {
        stringstream word_stream(line);
        string word;
        while (getline(word_stream, word, ' ')) {
            result.push_back(word);
            result.push_back(" ");
        }
        result.push_back("\n");
    }
    return result;
}

int main(int argc, const char* argv[]) {

    unordered_map<string, uint64_t> map;

    auto tokens = text_to_tokens(load_text_from_file("./test.txt"));
    uint64_t word_count_sum = tokens.size();

    for (string s : tokens) {

        if (map.find(s) == map.end()) {
            map[s] = 1;
        } else {
            map[s]++;
        }
    }

    if (argc == 2) {
        //compress
        if (strcmp(argv[1], "-c")) {

        }
    }

    priority_queue<TreeNode*, vector<TreeNode*>, CompareTreeNodes> q;

    for (auto const& [key, val] : map) {
        q.push(new TreeNode((float)val/(float)word_count_sum, key));
    }

    while (q.size() > 1) {
        TreeNode* temp1 = q.top();
        q.pop();
        TreeNode* temp2 = q.top();
        q.pop();
        q.push(new TreeNode(temp1->propability + temp2->propability, "", temp1, temp2));
    }

    TreeNode* final = q.top();
    final->encode_nodes();
    // final->print();

    unordered_map<string, string> codes;
    vector<TreeNode*> leafs;
    final->extract_leafs(leafs);
    delete final;
    for (auto leaf : leafs) {
        codes[leaf->coded_word] = leaf->code;
    }



    return 0;
}
