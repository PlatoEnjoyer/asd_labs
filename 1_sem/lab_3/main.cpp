#include <iostream>
#include <set>
#include <vector>
#include <cmath>
using namespace std;

int main() {
    long long x;
    cin >> x;

    set<long long> numbers;

    long long power3 = 1;
    for (int k = 0; power3 <= x; k++) {
        long long power5 = 1;
        for (int l = 0; power3 * power5 <= x; l++) {
            long long power7 = 1;
            for (int m = 0; power3 * power5 * power7 <= x; m++) {
                long long num = power3 * power5 * power7;
                numbers.insert(num);
                power7 *= 7;
            }
            power5 *= 5;
        }
        power3 *= 3;
    }

    for (long long num : numbers) {
        if (num >= 1) {
            cout << num << " ";
        }
    }
    cout << endl;

    return 0;
}
