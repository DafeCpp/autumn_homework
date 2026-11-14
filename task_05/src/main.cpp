#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>

struct Edge {
    int u, v;
    long long w;    
    bool incidentS;   
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

std::pair<long long, int> kruskalWithPenalty(
    const std::vector<Edge>& edges, 
    int n, 
    long long penalty
) {
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
        if (e.u == e.v) continue;
        
        long long eff = e.w + (e.incidentS ? penalty : 0);
        sortedEdges.push_back({eff, static_cast<int>(i), e.incidentS, e.u, e.v});
    }
    
    std::sort(sortedEdges.begin(), sortedEdges.end(), [](const SortableEdge& a, const SortableEdge& b) {
        if (a.effWeight != b.effWeight) return a.effWeight < b.effWeight;
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
    
    const long long MIN_PENALTY = -10000;
    const long long MAX_PENALTY = 10000;

    auto [minDegWeight, minDeg] = kruskalWithPenalty(edges, n, MAX_PENALTY);
    if (minDegWeight == -1) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
        return 0;
    }
    
    if (minDeg > d) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
        return 0;
    }
    
    long long lo = MIN_PENALTY, hi = MAX_PENALTY;
    long long bestWeight = LLONG_MAX;
    bool found = false;
    
    
    std::vector<std::pair<long long, int>> candidates; // (penalty, deg)
    
    while (lo <= hi) {
        long long mid = lo + (hi - lo) / 2;
        auto [weight, deg] = kruskalWithPenalty(edges, n, mid);
        
        if (weight == -1) {
            break;
        }
        
        candidates.push_back({mid, deg});
        
        if (deg <= d) {
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
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
