#include <iostream>
#include <string>
#include <stack>
#include <cctype>

struct Node {
    int data;
    Node* left;
    Node* right;

    explicit Node(int val) : data(val), left(nullptr), right(nullptr) {}
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
            ++pos; // '('
            node->left = parseNode(s, pos);

            if (pos < s.size() && s[pos] == ',') ++pos;

            node->right = parseNode(s, pos);

            if (pos < s.size() && s[pos] == ')') ++pos;
        }

        return node;
    }

public:
    Node* search(int value) {
        Node* current = root;
        while (current) {
            if (value == current->data) return current;
            current = (value < current->data) ? current->left : current->right;
        }
        return nullptr;
    }

    void insert(int value) {
        if (!root) {
            root = new Node(value);
            return;
        }

        Node* current = root;
        while (true) {
            if (value < current->data) {
                if (!current->left) {
                    current->left = new Node(value);
                    break;
                }
                current = current->left;
            } else if (value > current->data) {
                if (!current->right) {
                    current->right = new Node(value);
                    break;
                }
                current = current->right;
            } else {
                break; 
            }
        }
    }

    void remove(int value) {
        root = removeNode(root, value);
    }

private:
    Node* removeNode(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->data) {
            node->left = removeNode(node->left, value);
        } else if (value > node->data) {
            node->right = removeNode(node->right, value);
        } else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            } else {
                Node* successor = findMin(node->right);
                node->data = successor->data;
                node->right = removeNode(node->right, successor->data);
            }
        }
        return node;
    }

    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

public:
    std::string toParentheses() const {
        return root ? nodeToString(root) : "";
    }

private:
    std::string nodeToString(Node* node) const {
        if (!node) return "";

        std::string result = std::to_string(node->data);

        if (node->left || node->right) {
            result += "(";
            result += node->left ? nodeToString(node->left) : "";
            result += ",";
            result += node->right ? nodeToString(node->right) : "";
            result += ")";
        }

        return result;
    }
};


void showMenu() {
    std::cout << "\n--- Меню ---\n";
    std::cout << "1. Добавить элемент\n";
    std::cout << "2. Удалить элемент\n";
    std::cout << "3. Найти элемент\n";
    std::cout << "4. Вывести дерево (в скобочной записи)\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите операцию: ";
}

int main() {
    BST tree;
    std::string input;

    std::cout << "Введите дерево в линейно-скобочной записи: ";
    std::getline(std::cin, input);

    // Удаляем пробелы
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

    tree.buildFromParentheses(input);

    int choice, value;
    do {
        showMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                tree.insert(value);
                std::cout << "Добавлено: " << value << "\n";
                break;

            case 2:
                std::cout << "Введите значение для удаления: ";
                std::cin >> value;
                if (tree.search(value)) {
                    tree.remove(value);
                    std::cout << "Удалено: " << value << "\n";
                } else {
                    std::cout << "Элемент не найден.\n";
                }
                break;

            case 3:
                std::cout << "Введите значение для поиска: ";
                std::cin >> value;
                if (tree.search(value)) {
                    std::cout << "Элемент " << value << " найден.\n";
                } else {
                    std::cout << "Элемент " << value << " не найден.\n";
                }
                break;

            case 4:
                std::cout << "Текущее дерево: " << tree.toParentheses() << "\n";
                break;

            case 5:
                std::cout << "Выход. Финальное дерево: " << tree.toParentheses() << "\n";
                break;

            default:
                std::cout << "Неверный выбор.\n";
        }
    } while (choice != 5);

    return 0;
}