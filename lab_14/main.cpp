#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sstream>

class HashTable {
private:
    int tableSize;
    int count;
    double maxLoadFactor;
    std::vector<std::list<std::pair<std::string, std::string>>> table;

    int hash(const std::string& key) const {
        int sum = 0;
        for (char c : key) {
            sum += static_cast<int>(c);
        }
        return sum % tableSize;
    }

    void rehash() {
        int oldSize = tableSize;
        std::vector<std::list<std::pair<std::string, std::string>>> oldTable = std::move(table);

        tableSize *= 2;
        table.clear();
        table.resize(tableSize);
        count = 0;

        for (int i = 0; i < oldSize; ++i) {
            for (const auto& pair : oldTable[i]) {
                insert(pair.first, pair.second);
            }
        }
    }

public:
    explicit HashTable(int initialSize = 10, double loadFactor = 0.75)
        : tableSize(initialSize), count(0), maxLoadFactor(loadFactor) {
        table.resize(tableSize);
    }

    void insert(const std::string& key, const std::string& value) {
        if ((double)count / tableSize >= maxLoadFactor) {
            rehash();
        }

        int index = hash(key);
        auto& chain = table[index];

        for (auto& pair : chain) {
            if (pair.first == key) {
                pair.second = value;
                return;
            }
        }

        chain.push_back(std::make_pair(key, value));
        count++;
    }

    bool search(const std::string& key, std::string& value) const {
        int index = hash(key);
        const auto& chain = table[index];
        for (const auto& pair : chain) {
            if (pair.first == key) {
                value = pair.second;
                return true;
            }
        }
        return false;
    }

    bool remove(const std::string& key) {
        int index = hash(key);
        auto& chain = table[index];

        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == key) {
                chain.erase(it);
                count--;
                return true;
            }
        }
        return false;
    }

    void display(std::ostream& os) const {
        for (int i = 0; i < tableSize; ++i) {
            os << "[" << i << "]: ";
            bool first = true;
            for (const auto& pair : table[i]) {
                if (!first) os << ", ";
                os << "(" << pair.first << " -> " << pair.second << ")";
                first = false;
            }
            os << "\n";
        }
    }
};


void processFile(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename);
    std::ofstream outputFile(outputFilename);

    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть входной файл.\n";
        return;
    }
    if (!outputFile.is_open()) {
        std::cerr << "Ошибка: не удалось создать выходной файл.\n";
        return;
    }

    HashTable ht(5, 0.5);
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "INSERT") {
            std::string key, value;
            iss >> key >> value;
            ht.insert(key, value);
            outputFile << "INSERT: " << key << " -> " << value << "\n";
        }
        else if (command == "SEARCH") {
            std::string key;
            iss >> key;
            std::string value;
            if (ht.search(key, value)) {
                outputFile << "SEARCH " << key << ": найдено -> " << value << "\n";
            } else {
                outputFile << "SEARCH " << key << ": не найдено\n";
            }
        }
        else if (command == "REMOVE") {
            std::string key;
            iss >> key;
            if (ht.remove(key)) {
                outputFile << "REMOVE: ключ " << key << " удалён\n";
            } else {
                outputFile << "REMOVE: ключ " << key << " не найден\n";
            }
        }
        else if (command == "DISPLAY") {
            outputFile << "Текущее состояние хеш-таблицы:\n";
            ht.display(outputFile);
        }
        else {
            outputFile << "Неизвестная команда: " << command << "\n";
        }
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    processFile("input.txt", "output.txt");
    return 0;
}