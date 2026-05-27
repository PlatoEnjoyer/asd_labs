def egg_dropping(eggs, floors):
    """
    Возвращает минимальное количество бросков для eggs яиц и floors этажей
    """
    # dp[яиц][этажей] = мин. число бросков
    dp = [[0] * (floors + 1) for _ in range(eggs + 1)]
    
    # 1 яйцо: нужно проверить все этажи последовательно
    for f in range(1, floors + 1):
        dp[1][f] = f
    
    # 0 или 1 этаж: очевидные случаи
    for e in range(1, eggs + 1):
        dp[e][0] = 0   # 0 этажей -> 0 бросков
        dp[e][1] = 1   # 1 этаж -> 1 бросок
    
    # Заполняем таблицу для 2..eggs яиц и 2..floors этажей
    for e in range(2, eggs + 1):
        for f in range(2, floors + 1):
            dp[e][f] = float('inf')
            
            # Пробуем бросить с каждого этажа x
            for x in range(1, f + 1):
                # Яйцо разбилось: проверяем x-1 этажей внизу (осталось e-1 яиц)
                # Яйцо целое: проверяем f-x этажей вверху (осталось e яиц)
                worst = 1 + max(dp[e-1][x-1], dp[e][f-x])
                dp[e][f] = min(dp[e][f], worst)
    
    return dp[eggs][floors]


# Частный случай: 2 яйца, 100 этажей
print(f"2 яйца, 100 этажей: {egg_dropping(2, 100)} бросков")  # 14