#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

const string INPUT_FILE = "input.txt";
const string OUTPUT_FILE = "output.txt";
const string FILE1 = "f1.txt";
const string FILE2 = "f2.txt";
const string FILE3 = "f3.txt";

const int BUFFER_SIZE = 10;

struct FileBuffer {
    ifstream file;
    vector<int> buffer;
    int index;
    bool empty;

    FileBuffer(const string& filename) : index(0), empty(false) {
        file.open(filename, ios::in);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл: " << filename << endl;
            empty = true;
            return;
        }
        load();
    }

    void load() {
        buffer.clear();
        int value;
        for (int i = 0; i < BUFFER_SIZE && file >> value; ++i) {
            buffer.push_back(value);
        }
        sort(buffer.begin(), buffer.end());
        index = 0;
        if (buffer.empty()) {
            empty = true;
        }
    }

    bool hasNext() {
        return !empty && index < buffer.size();
    }

    int next() {
        if (!hasNext()) return 0;
        int val = buffer[index++];
        if (index >= buffer.size()) {
            load(); 
        }
        return val;
    }

    ~FileBuffer() {
        if (file.is_open()) file.close();
    }
};


void distributionPhase(const string& inputFile) {
    ifstream input(inputFile);
    if (!input.is_open()) {
        cerr << "Ошибка: не удалось открыть входной файл!" << endl;
        exit(1);
    }

    ofstream f1(FILE1), f2(FILE2), f3(FILE3);
    vector<ofstream*> files = {&f1, &f2, &f3};
    int fileIndex = 0;

    vector<int> block;
    int value;

    while (input >> value) {
        block.push_back(value);
        if (block.size() == BUFFER_SIZE) {
            sort(block.begin(), block.end());
            for (int x : block) {
                *(files[fileIndex]) << x << " ";
            }
            *(files[fileIndex]) << "\n"; 
            block.clear();
            fileIndex = (fileIndex + 1) % 3;
        }
    }


    if (!block.empty()) {
        sort(block.begin(), block.end());
        for (int x : block) {
            *(files[fileIndex]) << x << " ";
        }
        *(files[fileIndex]) << "\n";
    }

    input.close();
    f1.close(); f2.close(); f3.close();
}


void mergePhase() {
    FileBuffer fb1(FILE1), fb2(FILE2), fb3(FILE3);
    ofstream output(OUTPUT_FILE);

    priority_queue<int, vector<int>, greater<int>> minHeap;

    auto addIfHasNext = [&](FileBuffer& fb) {
        while (fb.hasNext()) {
            minHeap.push(fb.next());
        }
    };

    addIfHasNext(fb1);
    addIfHasNext(fb2);
    addIfHasNext(fb3);

    while (!minHeap.empty()) {
        output << minHeap.top() << " ";
        minHeap.pop();
    }

    output.close();
}


void generateInput(int n) {
    ofstream input(INPUT_FILE);
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        input << rand() % 1000 << " ";
    }
    input.close();
}

int main() {
    cout << "Генерация тестовых данных" << endl;
    generateInput(50);

    cout << "Распределение по файлам" << endl;
    distributionPhase(INPUT_FILE);

    cout << "Слияние цепочек" << endl;
    mergePhase();

    cout << "Сортировка завершена. Результат в " << OUTPUT_FILE << endl;

    ifstream out(OUTPUT_FILE);
    cout << "Результат:\n";
    int x;
    while (out >> x) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}