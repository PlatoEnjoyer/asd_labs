"""
Дано: Контейнеры (ящики) одинаковой вместимости C,
Набор предметов с весами w₁, w₂, ..., wₙ (каждый вес ≤ C)

Найти: Минимальное количество ящиков, чтобы упаковать все предметы
"""



def exact_bin_packing(items, capacity):
    """
    Точное решение через DFS
    """
    items = sorted(items, reverse=True)
    n = len(items)
    best = n  # в худшем случае — каждый предмет в отдельном ящике
    
    def dfs(idx, bins_used, bins_remain):
        nonlocal best
        
        # Отсечение: если уже использовали больше best — смысла нет
        if bins_used >= best:
            return
        
        if idx == n:
            best = min(best, bins_used)
            return
        
        # Пробуем положить в существующие ящики
        for i in range(bins_used):
            if bins_remain[i] >= items[idx]:
                bins_remain[i] -= items[idx]
                dfs(idx + 1, bins_used, bins_remain)
                bins_remain[i] += items[idx]
        
        # Пробуем создать новый ящик
        bins_remain[bins_used] = capacity - items[idx]
        dfs(idx + 1, bins_used + 1, bins_remain)
    
    # bins_remain хранит оставшееся место в каждом ящике
    dfs(0, 0, [0] * n)
    return best


items_small = [4, 5, 3, 8, 2, 1, 7]
capacity = 10
min_bins = exact_bin_packing(items_small, capacity)
print(f"Точное решение: {min_bins} ящиков")