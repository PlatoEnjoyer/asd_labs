#include <iostream>
#include <vector>
using namespace std;

// Сортировка Шелла с последовательностью Кнута (1, 4, 13, 40, ...)
void shellSort(vector<int>& arr) {
    int n = arr.size();
    
    // Вычисляем начальный шаг по формуле Кнута: h = 3*h + 1
    int gap = 1;
    while (gap < n / 3) {
        gap = gap * 3 + 1;  // 1, 4, 13, 40, 121, ...
    }

    // Основной цикл: уменьшаем шаг
    for (; gap > 0; gap /= 3) {
        // Сортировка вставками с шагом gap
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            // Сдвигаем элементы, пока не найдём правильное место
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
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

    shellSort(numbers);

    cout << "Отсортированная последовательность: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}