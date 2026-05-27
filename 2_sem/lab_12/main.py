"""
Дано: Рюкзак вместимостью W (максимальный вес) 
n предметов, у каждого: weight[i] — вес, value[i] — ценность

Найти: Набор предметов (каждый можно взять не более одного раза), такой что: Суммарный вес ≤ W и суммарная ценность максимальна
"""




def knapsack_dp(weights, values, capacity):
    """
    Решение задачи о рюкзаке методом динамического программирования
    
    Args:
        weights: список весов предметов
        values: список ценностей предметов  
        capacity: вместимость рюкзака
    
    Returns:
        tuple: (максимальная ценность, список выбранных предметов)
    """
    n = len(weights)
    
    # Создаём таблицу DP
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]
    
    # Заполняем таблицу
    for i in range(1, n + 1):
        for w in range(capacity + 1):
            if weights[i-1] <= w:
                dp[i][w] = max(
                    values[i-1] + dp[i-1][w - weights[i-1]],  # Берём предмет
                    dp[i-1][w]  # Не берём предмет
                )
            else:
                dp[i][w] = dp[i-1][w]
    
    # Восстанавливаем выбранные предметы
    selected = []
    w = capacity
    for i in range(n, 0, -1):
        if dp[i][w] != dp[i-1][w]:
            selected.append(i-1)  # индекс предмета
            w -= weights[i-1]
    
    selected.reverse()  # восстанавливаем порядок
    
    return dp[n][capacity], selected


# Пример использования
weights = [2, 3, 4, 5]
values = [3, 4, 5, 6]
capacity = 5

max_value, selected_items = knapsack_dp(weights, values, capacity)
print(f"Максимальная ценность: {max_value}")
print(f"Выбранные предметы (индексы): {selected_items}")
print(f"Их веса: {[weights[i] for i in selected_items]}")
print(f"Их ценности: {[values[i] for i in selected_items]}")