#include <iostream>
#include <vector>
using namespace std;

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;          
    int left = 2 * i + 1;      
    int right = 2 * i + 2; 

    // Если левый потомок существует и больше корня
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // Если правый потомок существует и больше текущего максимума
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Если наибольший элемент — не корень, то меняем и просеиваем дальше
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);  // рекурсивно просеиваем поддерево
    }
}


void heapSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    int n;
    cout << "Введите количество чисел: ";
    cin >> n;

    vector<int> numbers(n);
    cout << "Введите " << n << " чисел: ";
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    heapSort(numbers);

    cout << "Отсортированная последовательность: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}