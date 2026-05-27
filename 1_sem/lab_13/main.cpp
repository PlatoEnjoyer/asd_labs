#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

enum class EntryState {
    EMPTY,
    OCCUPIED,
    DELETED
};

struct Entry {
    std::string key;
    std::string value;
    EntryState state;

    Entry() : state(EntryState::EMPTY) {}
};

class HashTable {
private:
    int tableSize;
    int count; 
    double maxLoadFactor;
    std::vector<Entry> table;

    int hash(const std::string& key) const {
        int sum = 0;
        for (char c : key) {
            sum += static_cast<int>(c);
        }
        return sum % tableSize;
    }

    int findIndex(const std::string& key) const {
        int index = hash(key);
        int start = index;
        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                return index;
            }
            if (table[index].state == EntryState::EMPTY) {
                return -1;
            }
            index = (index + 1) % tableSize;
        } while (index != start);
        return -1;
    }

    int findEmptyOrDeleted(const std::string& key) {
        int index = hash(key);
        int start = index;
        int firstDeleted = -1;

        do {
            if (table[index].state == EntryState::OCCUPIED && table[index].key == key) {
                return index;
            }
            if (table[index].state == EntryState::DELETED && firstDeleted == -1) {
                firstDeleted = index;
            }
            if (table[index].state == EntryState::EMPTY) {
                return index;
            }
            index = (index + 1) % tableSize;
        } while (index != start);

        return firstDeleted;
    }

    void rehash() {
        int oldSize = tableSize;
        std::vector<Entry> oldTable = std::move(table);

        tableSize *= 2; 
        table.clear();
        table.resize(tableSize);
        count = 0;

        for (int i = 0; i < oldSize; ++i) {
            if (oldTable[i].state == EntryState::OCCUPIED) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }

public:
    explicit HashTable(int initialSize = 10, double loadFactor = 0.75)
        : tableSize(initialSize), count(0), maxLoadFactor(loadFactor) {
        table.resize(tableSize);
        for (auto& entry : table) {
            entry.state = EntryState::EMPTY;
        }
    }

    void insert(const std::string& key, const std::string& value) {
        if ((double)count / tableSize >= maxLoadFactor) {
            rehash();
        }

        int index = findEmptyOrDeleted(key);
        if (index == -1) {
            std::cerr << "Таблица переполнена: не удаётся вставить ключ '" << key << "'\n";
            return;
        }

        if (table[index].state != EntryState::OCCUPIED || table[index].key != key) {
            count++; 
        }

        table[index].key = key;
        table[index].value = value;
        table[index].state = EntryState::OCCUPIED;
    }

    bool search(const std::string& key, std::string& value) const {
        int index = findIndex(key);
        if (index != -1) {
            value = table[index].value;
            return true;
        }
        return false;
    }

    bool remove(const std::string& key) {
        int index = findIndex(key);
        if (index != -1) {
            table[index].state = EntryState::DELETED;
            count--;
            return true;
        }
        return false;
    }

    void display(std::ostream& os) const {
        for (int i = 0; i < tableSize; ++i) {
            os << "[" << i << "]: ";
            if (table[i].state == EntryState::OCCUPIED) {
                os << "(" << table[i].key << " -> " << table[i].value << ")";
            } else if (table[i].state == EntryState::DELETED) {
                os << "<deleted>";
            } else {
                os << "<empty>";
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