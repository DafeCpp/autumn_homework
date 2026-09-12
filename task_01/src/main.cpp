#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


void create(int m, vector<vector<int>> &vect) {
    for (int i = 0; i < m; i++) {
        int first, second;
        cin >> first >> second;

        if (first == second) {
            cout << -1;               // найдена циклическая зависимость
            return;
        }
        
        vect[first].push_back(second);
    }
}

void dfs(int u, vector<vector<int>> &vect, vector<char> &color, vector<int> &sorted, bool &has_cycle) {
    if (color[u] == 'b') return;
    color[u] = 'g';

    for (int v : vect[u]) {
        if (color[v] == 'b') {
            has_cycle = true;
            return;
        }
        if (color[u] == 'w') {
            dfs(v, vect, color, sorted, has_cycle);
            if (has_cycle) return;
        }
    }

    color[u] = 'b';
    sorted.push_back(u);
}


int main() {
    unsigned int n, m;
    cin >> n >> m;

    vector<vector<int>> vect(n, vector<int>());

    create(m, vect);

    vector<char> color(n, 'w');
    vector<int> sorted;
    bool has_cycle = false;

    for (int i = 0; i < n; i++) {
        if (color[i] == 'w') {
            dfs(i, vect, color, sorted, has_cycle);
            if (has_cycle) {
                cout << -1 << endl;
                return 0;
            }
        }
    }

    reverse(sorted.begin(), sorted.end());

    cout << endl;
    for (int elem : sorted)
        cout << elem << " ";

    return 0;
}
