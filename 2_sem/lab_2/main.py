import itertools
import math


def cross_product(o, a, b):
    '''Функция векторного произведения векторов OA и OB'''
    return (a[0] - o[0]) * (b[1] - o[1]) - (a[1] - o[1]) * (b[0] - o[0])


def is_point_in_triangle(p, a, b, c):
    '''a,b,c - вершины треугольника. 
    Функция проверяет принадлежность p с помощью векторного произведения'''
    cp1 = cross_product(a, b, p)
    cp2 = cross_product(b, c, p)
    cp3 = cross_product(c, a, p)

    # Случай когда p лежит на границе тр-ка abc
    if cp1 == 0 or cp2 == 0 or cp3 == 0:
        return False
    
    # Точка p внутри если знаки всех произведений одинаковы
    return (cp1 > 0 and cp2 > 0 and cp3 > 0) or (cp1 < 0 and cp2 < 0 and cp3 < 0)


def degenerate_case(a, b, c):
    '''Случай точек, лежащих на одной прямой'''
    return cross_product(a, b, c) == 0;


def find_nested_triangles(points):
    '''Функция поиска пару треугольников, один из которых вложен в другой'''
    n = len(points)
    if n < 6:
        pass
    
    # Генерация всевозможных треугольников из точек
    triangles = []
    for combo in itertools.combinations(range(n), 3):
        i, j, k = combo
        a, b, c = points[i], points[j], points[k]

        if not degenerate_case(a, b, c):
            triangles.append((i, j, k))
        
    # Перебор всех пар треугольников
    for t1_idx in range(len(triangles)):
        for t2_idx in range(len(triangles)):
            if t1_idx == t2_idx:
                continue
                
            idx1 = triangles[t1_idx]
            idx2 = triangles[t2_idx]

            t1_pts = [points[idx1[0]], points[idx1[1]], points[idx1[2]]]
            t2_pts = [points[idx2[0]], points[idx2[1]], points[idx2[2]]]
            # Проверка свойства: каждая вершина t2 лежит внутри t1
            p1_in = is_point_in_triangle(t2_pts[0], t1_pts[0], t1_pts[1], t1_pts[2])
            p2_in = is_point_in_triangle(t2_pts[1], t1_pts[0], t1_pts[1], t1_pts[2])
            p3_in = is_point_in_triangle(t2_pts[2], t1_pts[0], t1_pts[1], t1_pts[2])

            if p1_in and p2_in and p3_in:
                print(f"Внешний треугольник {t1_pts}")
                print(f"Внутренний треугольник {t2_pts}")
                return True
            
    return False


# тест 1, вложенные треугольники есть
points_example_1 = [
    (0, 0), (10, 0), (0, 10),  # Вершины большого
    (1, 1), (2, 1), (1, 2),    # Вершины малого
    (5, 5), (8, 1)             # Прочие точки
]

print("тест 1, ожидаем true")
result1 = find_nested_triangles(points_example_1)
print(result1)

# тест 2, нет вложенных треугольников
points_example_2 = [
    (0, 0), (1, 0), (0, 1),
    (2, 2), (3, 2), (2, 3)
]

print("тест 2, ожидаем false")
result2 = find_nested_triangles(points_example_2)
print(result2)


# тест 3, вырожденный случай
points_example_3 = [
    (0, 0), (1, 1), (2, 2),
    (0, 1), (1, 0), (2, 0)
]
print("тест 3, ожидаем false")
result3 = find_nested_triangles(points_example_3)
print(result3)