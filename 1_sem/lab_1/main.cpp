#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

int main() {
    vector<char> stack;
    map<char, char> pairs;
    pairs['('] = ')';
    pairs['['] = ']';
    pairs['{'] = '}';
    bool flag = true;
    string s;
    cin >> s;

    for (char el : s) {
        if (pairs.count(el)) {
            stack.push_back(el);
        } else {
            if (stack.empty() || pairs[stack.back()] != el) {
                flag = false;
                break;
            }
            stack.pop_back();
        }
    }

    if (flag && stack.empty()) {
        cout << "Последовательность корректна";
    } else {
        cout << "Последовательность некорректна";
    }

    return 0;
}
