import subprocess

tests = [
    ("()", "корректна"),
    ("[]", "корректна"),
    ("{}", "корректна"),
    ("([{}])", "корректна"),
    ("((()))", "корректна"),
    ("(", "некорректна"),
    (")", "некорректна"),
    ("([)]", "некорректна"),
    ("}{", "некорректна"),
    ("", "корректна"),
]

ok = True
for inp, expected in tests:
    result = subprocess.run(
        ["./build/debug/lab_1.exe"],
        input=inp,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding="utf-8",
        timeout=5
    )
    output = result.stdout.strip()
    if expected not in output:
        print(f"ОШИБКА: ввод='{inp}'")
        print(f"   Ожидалось: '{expected}'")
        print(f"   Получено:  '{output}'")
        ok = False

if ok:
    print("Все тесты прошли успешно!")
else:
    print("Есть ошибки.")
