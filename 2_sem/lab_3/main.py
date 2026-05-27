def build_dfa(pattern: str, alphabet: set = None) -> list:
    """
    Строит таблицу переходов DFA
    Возвращает список словарей: dfa[state][char] = next_state
    """
    m = len(pattern)
    if alphabet is None:
        alphabet = set(pattern)
    
    # Создаем таблицу переходов размером (m+1) x |alphabet|
    # Используем список словарей
    dfa = [dict() for _ in range(m + 1)]
    
    # переменная для эффективного отката при неудачном переходе
    restart_state = 0
    
    # Перебор всех состояний
    for q in range(m + 1):
        # Перебор каждого символо в алфавите для каждого состояния
        for char in alphabet:
            # Если нашли очередное совпадение с паттерном
            if q < m and char == pattern[q]:
                dfa[q][char] = q + 1
            else:
                if q == 0:
                    dfa[q][char] = 0
                else:
                    # Откатываемся к restart_state и смотрим переход оттуда
                    dfa[q][char] = dfa[restart_state].get(char, 0)
        
        # Обновляем restart_state для следующего q
        if q < m:
            restart_state = dfa[restart_state].get(pattern[q], 0)
            
    return dfa


def dfa_search(text: str, pattern: str, alphabet: set = None) -> list:
    if not pattern or not text:
        return []
    
    m = len(pattern)
    n = len(text)
    
    dfa = build_dfa(pattern, alphabet)
    
    occurrences = []
    current_state = 0
    
    # Перебор всех символов текста
    for i in range(n):
        char = text[i]
        current_state = dfa[current_state].get(char, 0)
        
        # Нашли совпадение
        if current_state == m:
            start_index = i - m + 1
            occurrences.append(start_index)
    
    return occurrences



if __name__ == "__main__":
    text = "ABABCABABABCABABABC"
    pattern = "ABABC"
    
    print(f"Текст: {text}")
    print(f"Образец: {pattern}")
    
    positions = dfa_search(text, pattern)
    print(f"Вхождения найдены на позициях: {positions}")
    
    # Проверка
    for pos in positions:
        print(f"  Подстрока с позиции {pos}: '{text[pos:pos+len(pattern)]}'")