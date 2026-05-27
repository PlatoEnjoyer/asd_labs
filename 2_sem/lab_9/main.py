""" n городов (вершин)

Матрица расстояний dist[i][j] — расстояние между городами i и j

dist[i][j] = dist[j][i]

dist[i][i] = 0 

Найти: Кратчайший замкнутый маршрут, который:

Посещает каждый город ровно один раз
Возвращается в исходный город
"""



def tsp(dist):
    """
    TSP через динамическое программирование
    """
    n = len(dist)
    
    # dp[посещенные_города][последний_город] = минимальная стоимость
    # посещенные_города храним как битовую маску
    INF = 10**9
    dp = [[INF] * n for _ in range(1 << n)]
    
    # Начинаем в городе 0
    dp[1 << 0][0] = 0  # 1 << 0 = 1 (посещен только город 0)
    
    # Перебираем все возможные множества посещенных городов
    for mask in range(1 << n):
        for last in range(n):
            if not (mask & (1 << last)):  # last не в mask? пропускаем
                continue
            if dp[mask][last] == INF:     # нет пути? пропускаем
                continue
            
            # Пробуем пойти в следующий город
            for nxt in range(n):
                if mask & (1 << nxt):     # город уже посещен? пропускаем
                    continue
                
                new_mask = mask | (1 << nxt)
                new_cost = dp[mask][last] + dist[last][nxt]
                
                if new_cost < dp[new_mask][nxt]:
                    dp[new_mask][nxt] = new_cost
    
    # Возвращаемся в начальный город
    full_mask = (1 << n) - 1  # все биты = 1
    answer = INF
    for last in range(1, n):  # last может быть любым, кроме 0
        if dp[full_mask][last] < INF:
            answer = min(answer, dp[full_mask][last] + dist[last][0])
    
    return answer


# Пример
dist1 = [
    [0, 10, 15],
    [10, 0, 20],
    [15, 20, 0]
]

cost = tsp(dist1)
print(f"Минимальная стоимость: {cost}")