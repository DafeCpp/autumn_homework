#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::ios_base;
using std::pair;
using std::vector;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<pair<int, int>> edges(m);
  for (int i = 0; i < m; ++i) {
    cin >> edges[i].first >> edges[i].second;
  }

  cout << MinEdgesToStronglyConnected(n, edges) << endl;

  return 0;
}
