#include <iostream>
#include <vector>
#include <algorithm> 
using namespace std;

void combSort(vector<int>& arr) {
    int n = arr.size();
    int gap = n;
    const float shrink = 1.3f;
    bool swapped = true;

    while (gap > 1 || swapped) {
        gap = static_cast<int>(gap / shrink);
        if (gap < 1) {
            gap = 1;
        }

        swapped = false;

        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
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

    combSort(numbers);

    cout << "Отсортированная последовательность: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}