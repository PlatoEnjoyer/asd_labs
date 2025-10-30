#include <iostream>
#include <vector>
using namespace std;


int partition(vector<int>& arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1; 
}


void quickSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int pivotIndex = partition(arr, left, right);

        quickSort(arr, left, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, right);
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

    quickSort(numbers, 0, n - 1);

    cout << "Отсортированная последовательность: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}