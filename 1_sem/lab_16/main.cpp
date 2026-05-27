#include <iostream>
#include <stack>
#include <string>

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root;
    BST() : root(nullptr) {}

    void insert(int value) {
        Node*& node = findPlace(root, value);
        node = new Node(value);
    }

private:
    Node*& findPlace(Node*& n, int value) {
        if (!n || n->data == value) return n;
        return (value < n->data) ? findPlace(n->left, value)
                                 : findPlace(n->right, value);
    }

public:
    std::string preorderTraversal() const {
        std::string result;
        std::stack<Node*> stack;

        if (root) stack.push(root);

        while (!stack.empty()) {
            Node* current = stack.top();
            stack.pop();

            if (current) {
                result += std::to_string(current->data) + " ";

                if (current->right) stack.push(current->right);
                if (current->left)  stack.push(current->left);
            }
        }

        return result;
    }
};

int main() {
    BST tree;
    int values[] = {8, 3, 10, 1, 6, 4, 7, 14, 13};

    for (int v : values) {
        tree.insert(v);
    }

    std::cout << "Прямой обход (без рекурсии): " << tree.preorderTraversal() << "\n";
    return 0;
}