import math
from functools import cmp_to_key


def is_zero(x):
    return abs(x) < 1e-10

def vec(x1, y1, x2, y2):
    return (x2 - x1, y2 - y1)

def cross(ax, ay, bx, by):
    return ax * by - ay * bx

def dot(ax, ay, bx, by):
    return ax * bx + ay * by

def point_on_segment(px, py, x1, y1, x2, y2):
    """Проверяет, лежит ли точка на отрезке (x1,y1)-(x2,y2)"""
    cross_val = cross(px - x1, py - y1, x2 - x1, y2 - y1)
    if not is_zero(cross_val):
        return False
    dot_val = dot(px - x1, py - y1, x2 - x1, y2 - y1)
    if dot_val < -1e-9:
        return False
    sq_len = (x2 - x1)**2 + (y2 - y1)**2
    if dot_val > sq_len + 1e-9:
        return False
    return True

def segments_intersect(x1, y1, x2, y2, x3, y3, x4, y4):
    """Проверяет, пересекаются ли отрезки (включая концы)"""
    def orient(px, py, qx, qy, rx, ry):
        val = cross(qx - px, qy - py, rx - px, ry - py)
        if is_zero(val):
            return 0
        return 1 if val > 0 else -1

    o1 = orient(x1, y1, x2, y2, x3, y3)
    o2 = orient(x1, y1, x2, y2, x4, y4)
    o3 = orient(x3, y3, x4, y4, x1, y1)
    o4 = orient(x3, y3, x4, y4, x2, y2)

    # Общий случай
    if o1 != o2 and o3 != o4:
        return True

    # Специальные случаи (коллинеарность)
    if o1 == 0 and point_on_segment(x3, y3, x1, y1, x2, y2):
        return True
    if o2 == 0 and point_on_segment(x4, y4, x1, y1, x2, y2):
        return True
    if o3 == 0 and point_on_segment(x1, y1, x3, y3, x4, y4):
        return True
    if o4 == 0 and point_on_segment(x2, y2, x3, y3, x4, y4):
        return True
    return False

# Алгоритм Грэхема для выпуклой оболочки

def graham_scan(points):
    """
    Возвращает список вершин выпуклой оболочки в порядке обхода против часовой стрелки.
    Если точек < 3 или все коллинеарны, возвращает [].
    """
    if len(points) < 3:
        return []

    # Находим самую левую нижнюю точку
    start = min(points, key=lambda p: (p[1], p[0]))
    points = list(points)

    # Сортируем по полярному углу относительно start
    def polar_angle(p):
        return math.atan2(p[1] - start[1], p[0] - start[0])

    points.sort(key=lambda p: (polar_angle(p), (p[0] - start[0])**2 + (p[1] - start[1])**2))

    # Убираем дубликаты (опционально)
    unique = []
    for p in points:
        if not unique or p != unique[-1]:
            unique.append(p)
    points = unique

    # Строим оболочку
    hull = []
    for p in points:
        while len(hull) >= 2:
            a = hull[-2]
            b = hull[-1]
            # Если поворот не против часовой стрелки, удаляем последнюю точку
            if cross(b[0] - a[0], b[1] - a[1], p[0] - b[0], p[1] - b[1]) <= 0:
                hull.pop()
            else:
                break
        hull.append(p)

    # Проверка на вырожденность (все точки на одной прямой)
    if len(hull) < 3:
        return []
    return hull

# 3. Пересечения


def line_intersection(a1, b1, c1, a2, b2, c2):
    """
    Пересечение двух прямых, заданных в виде Ax + By + C = 0.
    Возвращает (x, y) или None, если прямые параллельны.
    """
    det = a1 * b2 - a2 * b1
    if is_zero(det):
        return None
    x = (b1 * c2 - b2 * c1) / det
    y = (c1 * a2 - c2 * a1) / det
    return (x, y)

def line_segment_intersection(a, b, c, x1, y1, x2, y2):
    """
    Пересечение прямой Ax + By + C = 0 и отрезка (x1,y1)-(x2,y2).
    Возвращает список точек пересечения (0, 1 или 2 точки).
    """
    f1 = a * x1 + b * y1 + c
    f2 = a * x2 + b * y2 + c

    if is_zero(f1) and is_zero(f2):
        # Прямая совпадает с отрезком
        return [(x1, y1), (x2, y2)]  # можно вернуть все точки, но обычно концы
    if is_zero(f1):
        return [(x1, y1)]
    if is_zero(f2):
        return [(x2, y2)]

    if f1 * f2 > 0:
        return []

    t = -f1 / (f2 - f1)
    x = x1 + t * (x2 - x1)
    y = y1 + t * (y2 - y1)
    return [(x, y)]

def segment_intersection(x1, y1, x2, y2, x3, y3, x4, y4):
    """
    Пересечение двух отрезков.
    Возвращает список точек пересечения (0, 1 или бесконечно много - тогда список из двух концов общего участка).
    """
    if not segments_intersect(x1, y1, x2, y2, x3, y3, x4, y4):
        return []

    # Параметрическое решение
    A1 = y2 - y1
    B1 = x1 - x2
    C1 = A1 * x1 + B1 * y1

    A2 = y4 - y3
    B2 = x3 - x4
    C2 = A2 * x3 + B2 * y3

    det = A1 * B2 - A2 * B1

    if is_zero(det):
        # Коллинеарные отрезки — ищем пересечение проекций
        def on_segment(px, py):
            return point_on_segment(px, py, x1, y1, x2, y2) and point_on_segment(px, py, x3, y3, x4, y4)

        points = []
        for p in [(x1, y1), (x2, y2), (x3, y3), (x4, y4)]:
            if on_segment(p[0], p[1]) and p not in points:
                points.append(p)
        points.sort()
        return points if len(points) == 2 else points
    else:
        x = (C1 * B2 - C2 * B1) / det
        y = (A1 * C2 - A2 * C1) / det
        return [(x, y)]

def line_circle_intersection(a, b, c, cx, cy, r):
    """
    Пересечение прямой Ax + By + C = 0 и окружности (x-cx)^2 + (y-cy)^2 = r^2.
    Возвращает список точек пересечения (0, 1, 2 точки).
    """
    # Переносим систему так, чтобы центр окружности был в (0,0)
    # Уравнение прямой: A(x+cx) + B(y+cy) + C = 0 -> Ax + By + (A*cx + B*cy + C) = 0
    C2 = a * cx + b * cy + c
    # Теперь прямая: a x + b y + C2 = 0, окружность: x^2 + y^2 = r^2

    if is_zero(a) and is_zero(b):
        return []

    # Расстояние от центра до прямой
    dist = abs(C2) / math.hypot(a, b)
    if dist > r + 1e-9:
        return []
    if is_zero(a):
        # Прямая b y + C2 = 0 -> y = -C2/b
        y0 = -C2 / b
        dy = math.sqrt(max(0, r**2 - y0**2))
        return [(cx + dy, cy + y0), (cx - dy, cy + y0)] if dy > 1e-9 else [(cx, cy + y0)]
    if is_zero(b):
        x0 = -C2 / a
        dx = math.sqrt(max(0, r**2 - x0**2))
        return [(cx + x0, cy + dx), (cx + x0, cy - dx)] if dx > 1e-9 else [(cx + x0, cy)]

    # Общий случай
    # a x + b y + C2 = 0 -> y = (-a x - C2)/b
    # Подставляем в x^2 + y^2 = r^2
    # (a^2 + b^2) x^2 + 2 a C2 x + C2^2 - b^2 r^2 = 0
    A = a*a + b*b
    B = 2 * a * C2
    C = C2*C2 - b*b*r*r

    disc = B*B - 4*A*C
    if disc < -1e-9:
        return []
    if disc < 0:
        disc = 0
    sqrt_disc = math.sqrt(disc)

    x1 = (-B + sqrt_disc) / (2*A)
    x2 = (-B - sqrt_disc) / (2*A)

    y1 = (-a*x1 - C2)/b
    y2 = (-a*x2 - C2)/b

    points = []
    if sqrt_disc < 1e-9:
        points.append((cx + x1, cy + y1))
    else:
        points.append((cx + x1, cy + y1))
        points.append((cx + x2, cy + y2))
    return points

def segment_circle_intersection(x1, y1, x2, y2, cx, cy, r):
    """
    Пересечение отрезка и окружности.
    """
    # Сначала пересечение прямой через отрезок и окружности
    a = y2 - y1
    b = x1 - x2
    c = -a*x1 - b*y1  # ax + by + c = 0

    pts = line_circle_intersection(a, b, c, cx, cy, r)

    # Оставляем только те, что лежат на отрезке
    result = []
    for (x, y) in pts:
        if point_on_segment(x, y, x1, y1, x2, y2):
            result.append((x, y))
    # Убираем дубликаты
    unique = []
    for p in result:
        if not unique or math.hypot(p[0]-unique[-1][0], p[1]-unique[-1][1]) > 1e-9:
            unique.append(p)
    return unique

def circles_intersection(x1, y1, r1, x2, y2, r2):
    """
    Пересечение двух окружностей.
    Возвращает список точек пересечения (0, 1, 2).
    """
    dx = x2 - x1
    dy = y2 - y1
    d = math.hypot(dx, dy)

    if is_zero(d) and is_zero(r1 - r2):
        return "бесконечно много"  # Совпадают

    if d > r1 + r2 + 1e-9 or d < abs(r1 - r2) - 1e-9:
        return []

    if is_zero(d - (r1 + r2)) or is_zero(d - abs(r1 - r2)):
        # Одна точка (касание)
        t = r1 / d
        x = x1 + t * dx
        y = y1 + t * dy
        return [(x, y)]

    # Две точки
    a = (r1**2 - r2**2 + d**2) / (2*d)
    h = math.sqrt(max(0, r1**2 - a**2))

    xm = x1 + (dx * a) / d
    ym = y1 + (dy * a) / d

    x3 = xm + h * (dy) / d
    y3 = ym - h * (dx) / d
    x4 = xm - h * (dy) / d
    y4 = ym + h * (dx) / d

    return [(x3, y3), (x4, y4)]

def main():
    n = int(input("Введите количество точек N: "))
    points = []
    print("Введите координаты x y каждой точки:")
    for i in range(n):
        x, y = map(float, input().split())
        points.append((x, y))

    hull = graham_scan(points)

    if not hull:
        print("Выпуклой оболочки не существует (все точки коллинеарны или их меньше 3).")
    else:
        print("Выпуклая оболочка существует. Вершины (против часовой стрелки):")
        for p in hull:
            print(f"  ({p[0]:.3f}, {p[1]:.3f})")

    # Демонстрация остальных алгоритмов (по желанию)
    print("\n--- Примеры работы алгоритмов пересечений ---")
    print("Пересечение прямых 2x+3y-5=0 и x-y+2=0:",
          line_intersection(2, 3, -5, 1, -1, 2))
    print("Пересечение отрезка (0,0)-(2,2) и окружности (1,0, r=1):",
          segment_circle_intersection(0, 0, 2, 2, 1, 0, 1))
    print("Пересечение окружностей (0,0, r=3) и (4,0, r=3):",
          circles_intersection(0, 0, 3, 4, 0, 3))

if __name__ == "__main__":
    main()