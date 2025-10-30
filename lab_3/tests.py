import subprocess
import sys

TESTS = [
    ("1", [1]),
    ("2", [1]),
    ("3", [1, 3]),
    ("5", [1, 3, 5]),
    ("7", [1, 3, 5, 7]),
    ("10", [1, 3, 5, 7, 9]),
    ("15", [1, 3, 5, 7, 9, 15]),
    ("20", [1, 3, 5, 7, 9, 15]),
    ("30", [1, 3, 5, 7, 9, 15, 21, 25, 27]),
    ("50", [1, 3, 5, 7, 9, 15, 21, 25, 27, 35, 45, 49]),
]

def run_test(input_val):
    try:
        result = subprocess.run(
            ["./build/Debug/lab_3.exe"],
            input=input_val,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            encoding="utf-8",
            timeout=5
        )
        output = result.stdout.strip()
        error = result.stderr.strip()

        if result.returncode != 0:
            return "CRASH"
        if "error" in error.lower():
            return "ERROR"

        try:
            tokens = output.split()
            numbers = []
            for t in tokens:
                                t = t.strip()
                                if t:
                                    numbers.append(int(t))
            return sorted(numbers)
        except Exception:
            return "PARSE_ERROR"
    except subprocess.TimeoutExpired:
        return "TIMEOUT"
    except Exception as e:
        return "EXCEPTION"

def main():
    passed = 0
    total = len(TESTS)

    for i, (inp, expected) in enumerate(TESTS, 1):
        actual = run_test(inp)
        success = False

        if isinstance(actual, list) and actual == expected:
            success = True

        status = "Пройден" if success else "Провален"
        print(f"Test {i:2}: x={inp:<3} → {status}")

        if not success:
            expected_str = " ".join(map(str, expected))
            if isinstance(actual, list):
                actual_str = " ".join(map(str, actual))
            else:
                actual_str = str(actual)
            print(f"         Ожидалось: [{expected_str}]")
            print(f"         Получено:  [{actual_str}]")

        if success:
            passed += 1

    print("\n" + "="*60)
    print(f"Результат: {passed}/{total} тестов пройдено")
    if passed == total:
        print("Все тесты пройдены успешно.")
    else:
        print("Есть ошибки.")
        sys.exit(1)

if __name__ == "__main__":
    main()
