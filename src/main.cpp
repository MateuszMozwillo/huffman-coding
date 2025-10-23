#include <iostream>
#include <queue>

using namespace std;

struct TreeNode {
    double propability;
    string code;

    TreeNode *left;
    TreeNode *right;

    TreeNode(double p, TreeNode *l = nullptr, TreeNode *r = nullptr) {
        this->propability = p;
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

    void encode_nodes(string current_code = "") {
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
            cout << "\npropability: " << this->propability << "\ncode: " << this->code << "\n";
        }
        if (this->left != nullptr) {
            this->left->print();
        }
        if (this->right != nullptr) {
            this->right->print();
        }
    }
};

struct CompareTreeNodes {
    bool operator()(const TreeNode* a, const TreeNode* b) const {
        return a->propability > b->propability;
    }
};

int main() {

    priority_queue<TreeNode*, vector<TreeNode*>, CompareTreeNodes> q;

    q.push(new TreeNode(1./2.));
    q.push(new TreeNode(1./4.));
    q.push(new TreeNode(1./8.));
    q.push(new TreeNode(1./16.));
    q.push(new TreeNode(1./32.));
    q.push(new TreeNode(1./32.));

    while (q.size() > 1) {
        TreeNode* temp1 = q.top();
        q.pop();
        TreeNode* temp2 = q.top();
        q.pop();
        q.push(new TreeNode(temp1->propability + temp2->propability, temp1, temp2));
    }

    TreeNode* final = q.top();
    final->encode_nodes();
    final->print();

    delete final;

    return 0;
}
