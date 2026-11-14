// main.cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>

struct Edge {
    int u, v;
    long long w;       // исходный вес
    bool incidentS;     // ребро инцидентно вершине s
};

class DSU {
private:
    std::vector<int> parent, rank_;
public:
    DSU(int n) : parent(n), rank_(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rank_[x] < rank_[y]) std::swap(x, y);
        parent[y] = x;
        if (rank_[x] == rank_[y]) rank_[x]++;
        return true;
    }
};

// Запускает Kruskal с штрафом penalty для рёбер, инцидентных s.
// Возвращает пару: (истинный вес дерева, степень s в дереве).
// Если дерево не построено (граф несвязен после фильтрации петель), возвращает (-1, -1).
std::pair<long long, int> kruskalWithPenalty(
    const std::vector<Edge>& edges, 
    int n, 
    long long penalty
) {
    // Создаём копию рёбер с эффективными весами
    struct SortableEdge {
        long long effWeight;
        int originalIndex;
        bool incidentS;
        int u, v;
    };
    
    std::vector<SortableEdge> sortedEdges;
    sortedEdges.reserve(edges.size());
    
    for (size_t i = 0; i < edges.size(); ++i) {
        const auto& e = edges[i];
        // Петли пропускаем
        if (e.u == e.v) continue;
        
        long long eff = e.w + (e.incidentS ? penalty : 0);
        sortedEdges.push_back({eff, static_cast<int>(i), e.incidentS, e.u, e.v});
    }
    
    // Сортировка: сначала по эффективному весу, затем НЕ инцидентные s идут раньше
    // Это важно: при равных весах предпочитаем рёбра без штрафа, чтобы минимизировать deg(s)
    std::sort(sortedEdges.begin(), sortedEdges.end(), [](const SortableEdge& a, const SortableEdge& b) {
        if (a.effWeight != b.effWeight) return a.effWeight < b.effWeight;
        // При равных весах: не-incidentS идёт первым
        if (a.incidentS != b.incidentS) return !a.incidentS && b.incidentS;
        return a.originalIndex < b.originalIndex;
    });
    
    DSU dsu(n);
    long long totalEffWeight = 0;
    int edgeCount = 0;
    int degreeS = 0;
    long long actualWeight = 0;
    
    for (const auto& se : sortedEdges) {
        if (dsu.unite(se.u, se.v)) {
            totalEffWeight += se.effWeight;
            actualWeight += edges[se.originalIndex].w;
            if (se.incidentS) degreeS++;
            edgeCount++;
            if (edgeCount == n - 1) break;
        }
    }
    
    if (edgeCount != n - 1) {
        return {-1, -1}; // Граф несвязен
    }
    
    return {actualWeight, degreeS};
}

int main() {
    int n, m, s, d;
    if (!(std::cin >> n >> m >> s >> d)) return 0;
    
    // Переводим s в 0-based индексацию
    s--;
    
    std::vector<Edge> edges;
    edges.reserve(m);
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        std::cin >> u >> v >> w;
        u--; v--;
        
        bool incS = (u == s || v == s);
        edges.push_back({u, v, w, incS});
    }
    
    // Особый случай: n = 1
    if (n == 1) {
        std::cout << 0 << "\n";
        return 0;
    }
    
    // Диапазон для бинарного поиска по штрафу
    // Максимальный вес ребра = 1000, количество рёбер <= 10000
    // Штраф может быть отрицательным (чтобы увеличить deg(s)) или положительным (чтобы уменьшить)
    const long long MIN_PENALTY = -10000;
    const long long MAX_PENALTY = 10000;
    
    // Сначала проверим, возможно ли вообще построить дерево с deg(s) <= d
    // При большом положительном штрафе deg(s) минимален
    auto [minDegWeight, minDeg] = kruskalWithPenalty(edges, n, MAX_PENALTY);
    if (minDegWeight == -1) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
        return 0;
    }
    
    if (minDeg > d) {
        // Даже с максимальным штрафом степень слишком велика
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
        return 0;
    }
    
    // Теперь ищем минимальный штраф, при котором deg(s) <= d
    // Но нам нужен не просто любой штраф, а такой, что даёт минимальный ИСТИННЫЙ вес
    // среди всех деревьев с deg(s) <= d
    
    // Подход: бинарный поиск по penalty для нахождения критического значения,
    // вокруг которого deg(s) переходит через d.
    
    // Нам нужно найти такое penalty*, что:
    // - deg(s)(penalty*) <= d
    // - Для любого penalty < penalty*: deg(s)(penalty) >= deg(s)(penalty*) (монотонность)
    // - Среди всех деревьев с deg(s) <= d, дерево полученное при penalty* имеет минимальный истинный вес
    
    // Более надёжный подход: перебрать все возможные значения deg(s) от 0 до d,
    // и для каждого найти минимальный вес дерева с точно этим значением deg(s).
    // Но это сложно. 
    
    // Стандартный подход для этой задачи:
    // Выполнить бинарный поиск по penalty, чтобы найти границу, где deg(s) меняется с >d на <=d.
    // Тогда ответ = min(вес при penalty_low, вес при penalty_high), 
    // но нужно аккуратно восстановить истинный вес.
    
    // Давайте используем другой, более прямой метод:
    // Перебор числа рёбер incidentS от max(0, n-1-m_non_incident_max_possible) до min(d, n-1)
    // Для каждого фиксированного k (число рёбер incidentS в дереве):
    //   Найти MST с ровно k рёбрами incidentS. Это можно сделать, назначив всем incidentS рёбрам
    //   штраф X и подобрав X так, чтобы получилось ровно k. Или использовать DP.
    
    // Учитывая ограничения (n<=1000, m<=10000), можно использовать следующий трюк:
    // Отсортировать рёбра incidentS и non-incidentS отдельно.
    // Для каждого возможного k от 0 до min(d, count_incident_edges):
    //   Взять k наименьших incidentS рёбер и (n-1-k) наименьших non-incidentS рёбер,
    //   проверить связность через DSU... Но это не гарантирует MST!
    
    // Правильный стандартный алгоритм для "MST with degree constraint on one vertex":
    // Использовать параметрический поиск + восстановление ответа.
    
    // Реализуем бинарный поиск по penalty для нахождения оптимального решения.
    
    long long lo = MIN_PENALTY, hi = MAX_PENALTY;
    long long bestWeight = LLONG_MAX;
    bool found = false;
    
    // Мы хотим найти минимальный penalty, при котором deg(s) <= d.
    // Но поскольку deg(s) монотонно не возрастает с ростом penalty,
    // искомое множество penalty образует интервал [P*, +inf).
    // В точке P* происходит скачок deg(s) с d+1 на d (или меньше).
    // Ответ = вес дерева при любом penalty >= P*, но нужно взять минимум по истинному весу.
    
    // Проще: выполнить бинарный поиск, собрать все кандидаты near boundary.
    
    std::vector<std::pair<long long, int>> candidates; // (penalty, deg)
    
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        auto [weight, deg] = kruskalWithPenalty(edges, n, mid);
        
        if (weight == -1) {
            // Не должно происходить, граф связный
            break;
        }
        
        candidates.push_back({mid, deg});
        
        if (deg <= d) {
            // Можно попробовать меньший штраф (больше рёбер incidentS -> потенциально меньший вес?)
            // Нет: меньший штраф означает больше incidentS рёбер, но deg уже <= d.
            // Нам нужно исследовать область где deg <= d и найти минимальный истинный вес.
            // Поскольку при уменьшении штрафа deg растёт, а истинный вес может уменьшаться,
            // нужно искать левую границу области deg <= d.
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
    // Теперь у нас есть candidates. Нужно также проверить точки вокруг границы.
    // Добавим проверки для lo и lo+1 после выхода из цикла
    
    // Проверим несколько значений около границы
    std::set<long long> testPenalties;
    for (auto& c : candidates) {
        testPenalties.insert(c.first);
        testPenalties.insert(c.first - 1);
        testPenalties.insert(c.first + 1);
    }
    testPenalties.insert(MIN_PENALTY);
    testPenalties.insert(MAX_PENALTY);
    testPenalties.insert(lo);
    testPenalties.insert(hi);
    testPenalties.insert(lo - 1);
    testPenalties.insert(hi + 1);
    
    for (long long p : testPenalties) {
        if (p < MIN_PENALTY || p > MAX_PENALTY) continue;
        auto [weight, deg] = kruskalWithPenalty(edges, n, p);
        if (weight != -1 && deg <= d) {
            if (!found || weight < bestWeight) {
                bestWeight = weight;
                found = true;
            }
        }
    }
    
    if (!found) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
    } else {
        std::cout << bestWeight << "\n";
    }
    
    return 0;
}
