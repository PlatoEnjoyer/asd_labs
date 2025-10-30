#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void countingSortByDigit(vector<int>& arr, int exp) {
    int n = arr.size();
    vector<int> output(n);         
    vector<int> count(10, 0);    

    // Подсчитываем количество вхождений каждой цифры в текущем разряде
    for (int i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Преобразуем count в префиксную сумму
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    // Строим выходной массив (обратный проход для устойчивости)
    for (int i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // Копируем результат обратно в arr
    arr = output;
}

void radixSort(vector<int>& arr) {
    if (arr.empty()) return;

    // Находим максимальное число, чтобы определить количество разрядов
    int maxNum = *max_element(arr.begin(), arr.end());

    // Обрабатываем каждый разряд
    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        countingSortByDigit(arr, exp);
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

    radixSort(numbers);

    cout << "Отсортированная последовательность: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}