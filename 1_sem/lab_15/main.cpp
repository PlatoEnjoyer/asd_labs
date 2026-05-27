#include <iostream>
#include <string>
#include <cctype>

struct Node {
    int data;
    Node* left;
    Node* right;

    explicit Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root;

    BST() : root(nullptr) {}

    void buildFromParentheses(const std::string& s) {
        size_t pos = 0;
        root = parseNode(s, pos);
    }

private:
    Node* parseNode(const std::string& s, size_t& pos) {
        if (pos >= s.size()) return nullptr;

        int value = 0;
        bool foundDigit = false;
        while (pos < s.size() && std::isdigit(s[pos])) {
            value = value * 10 + (s[pos] - '0');
            ++pos;
            foundDigit = true;
        }
        if (!foundDigit) return nullptr;

        Node* node = new Node(value);

        if (pos < s.size() && s[pos] == '(') {
            ++pos;
            node->left = parseNode(s, pos);

            if (pos < s.size() && s[pos] == ',') {
                ++pos;
            }

            node->right = parseNode(s, pos);

            if (pos < s.size() && s[pos] == ')') {
                ++pos;
            }
        }

        return node;
    }

public:
    void preorder(Node* n) {
        if (n) {
            std::cout << n->data << " ";
            preorder(n->left);
            preorder(n->right);
        }
    }

    void inorder(Node* n) {
        if (n) {
            inorder(n->left);
            std::cout << n->data << " ";
            inorder(n->right);
        }
    }

    void postorder(Node* n) {
        if (n) {
            postorder(n->left);
            postorder(n->right);
            std::cout << n->data << " ";
        }
    }
};

int main() {
    BST tree;
    std::string input = "8(3(1,6(4,7)),10(,14(13,)))";

    tree.buildFromParentheses(input);

    std::cout << "Прямой обход:    "; tree.preorder(tree.root);   std::cout << "\n";
    std::cout << "Центральный:      "; tree.inorder(tree.root);    std::cout << "\n";
    std::cout << "Концевой обход:   "; tree.postorder(tree.root);  std::cout << "\n";

    return 0;
}