import subprocess
import sys

TESTS = [
    ("2+3=", 5.0),
    ("10-4=", 6.0),
    ("2*3*(3-3)=", 0.0),
    ("8/4=", 2.0),
    ("2+3*4=", 14.0),
    ("(2+3)*4=", 20.0),
    ("((2+3)*2)/5=", 2.0),
    ("-(5+3)=", -8.0),
    ("5*(-2)=", -10.0),
    ("3*(4-(2))=", 6.0),

    ("5/0=", "ERROR"),
    ("(5+3", "ERROR"),
    ("5+)= ", "ERROR"),
    ("abc=", "ERROR"),
    ("5+3", "ERROR"),
]


TOLERANCE = 1e-5

def run_test(expr):
    try:
        result = subprocess.run(
            ["./build/debug/lab_2.exe"],
            input=expr,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            encoding="utf-8",
            timeout=5
        )
        output = result.stdout.strip()
        error = result.stderr.strip()

        if result.returncode != 0:
            return "CRASH"
        if "ERROR" in output.upper():
            return "ERROR"
        try:
            return float(output)
        except ValueError:
            return "ERROR"
    except subprocess.TimeoutExpired:
        return "TIMEOUT"
    except Exception as e:
        return "EXCEPTION"

def approximate_equal(a, b):
    return abs(a - b) < TOLERANCE

def main():
    passed = 0
    total = len(TESTS)

    for i, (expr, expected) in enumerate(TESTS, 1):
        actual = run_test(expr)

        if isinstance(expected, str) and expected == "ERROR":
            success = (actual == "ERROR")
        elif isinstance(expected, (int, float)):
            success = (isinstance(actual, (int, float)) and approximate_equal(actual, expected))
        else:
            success = False

        status = "Пройден" if success else "Провален"
        print(f"Test {i:2}: {expr:<15} → {status}")

        if not success:
            print(f"         Ожидалось: {expected}, Получено: {actual}")

        if success:
            passed += 1

    print("\n" + "="*50)
    print(f"Результат: {passed}/{total} тестов пройдено")
    if passed == total:
        print("Все тесты пройдены успешно")
    else:
        print("Есть ошибки")
        sys.exit(1)

if __name__ == "__main__":
    main()
