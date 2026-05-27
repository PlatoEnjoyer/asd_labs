#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Calculator {
private:
    string expr;
    size_t pos = 0;

    void skipSpaces() {
        while (pos < expr.size() && isspace(expr[pos])) pos++;
    }

    double parseNumber() {
        skipSpaces();
        if (pos >= expr.size()) throw runtime_error("Ожидалось число");

        size_t start = pos;
        if (expr[pos] == '-') pos++;

        if (!isdigit(expr[pos]) && expr[pos] != '.')
            throw runtime_error("Неверный формат числа");

        while (pos < expr.size() && (isdigit(expr[pos]) || expr[pos] == '.'))
            pos++;

        return stod(expr.substr(start, pos - start));
    }

    // Для разбора атомарных единиц (чисел или выражений в скобках)
    double parseFactor() {
        skipSpaces();
        if (expr[pos] == '-') {
            pos++;
            return -parseFactor();
        }
        if (expr[pos] == '+') {
            pos++;
            return parseFactor();
        }

        if (expr[pos] == '(') {
            pos++;
            double value = parseExpression();
            skipSpaces();
            if (pos >= expr.size() || expr[pos] != ')') {
                throw runtime_error("Ожидалась ')' ");
            }
            pos++;
            return value;
        } else {
            return parseNumber();
        }
    }

    // Для разбора умножения и деления атомарных единиц
    double parseTerm() {
        double left = parseFactor();
        skipSpaces();

        while (pos < expr.size()) {
            char op = expr[pos];
            if ((op == '*' || op == '/') && pos < expr.size()) {
                pos++;
                double right = parseFactor();
                if (op == '*') left = left * right;
                else {
                    if (abs(right) < 1e-9)
                        throw runtime_error("Деление на ноль");
                    left = left / right;
                }
                skipSpaces();
            } else {
                break;
            }
        }

        return left;
    }

    // Для разбора сложения и вычитания атомарных единиц
    double parseExpression() {
        double left = parseTerm();
        skipSpaces();

        while (pos < expr.size()) {
            char op = expr[pos];
            if ((op == '+' || op == '-') && pos < expr.size()) {
                pos++;
                double right = parseTerm();
                if (op == '+') left = left + right;
                else left = left - right;
                skipSpaces();
            } else {
                break;
            }
        }

        return left;
    }

public:
    double calculate(const string& input) {
        expr = input;
        pos = 0;

        if (expr.empty()) {
        throw invalid_argument("Пустое выражение");
        }

        if (expr.back() != '=') {
            throw invalid_argument("Выражение должно заканчиваться на '='");
        }
        expr.pop_back();

        if (expr.empty()) {
        throw invalid_argument("Пустое выражение после '='");
        }

        double result = parseExpression();

        skipSpaces();
        if (pos < expr.size()) {
            throw invalid_argument("Неожиданный символ после выражения");
        }

        return result;
    }
};


int main() {
    string input;
    if (!getline(cin, input)) {
        return 1;
    }

    try {
        Calculator calc;
        double result = calc.calculate(input);
        cout << result << endl;
    } catch (const exception& e) {
        cout << "ERROR" << endl;
    }
    return 0;
}
