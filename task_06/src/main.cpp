#include <iostream>

#include "task6.h"
using namespace std;

int main() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> edges(m, vector<int>(3));
  for (int i = 0; i < m; ++i) {
    cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
  }
  cout << task6(n, m, edges, 1, n) << "\n";
  return 0;
}