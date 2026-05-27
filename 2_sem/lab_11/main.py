"""
Дано: Неориентированный граф G = (V, E) 
Найти: Минимальное количество цветов k, такое что каждую вершину можно покрасить в 
один из k цветов, и никакие две смежные вершины не имеют одинаковый цвет.
"""




def greedy_coloring(graph):
    """
    graph: список смежности
    возвращает: массив цветов для каждой вершины
    """
    n = len(graph)
    colors = [-1] * n
    
    for v in range(n):
        # Множество занятых цветов у соседей
        used = set()
        for neighbor in graph[v]:
            if colors[neighbor] != -1:
                used.add(colors[neighbor])
        
        # Берём первый свободный цвет
        color = 0
        while color in used:
            color += 1
        colors[v] = color
    
    return colors, max(colors) + 1


# Пример
graph = [
    [1, 2],     
    [0, 2],     
    [0, 1],     
    [4],       
    [3]       
]

colors, num_colors = greedy_coloring(graph)
print(f"Цвета: {colors}")
print(f"Использовано цветов: {num_colors}")