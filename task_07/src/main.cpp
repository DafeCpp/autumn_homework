#include <iostream>
#include <vector>

#include "task7.h"
using namespace std;
int main() {
  int n, q;
  cin >> n >> q;

  vector<int> a(n + 1, 0);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  SegmentTree st(a);

  for (int i = 0; i < q; i++) {
    int type;
    cin >> type;
    if (type == 1) {
      int l, r;
      cin >> l >> r;
      cout << st.query(l, r) << "\n";
    } else if (type == 2) {
      int idx, val;
      cin >> idx >> val;
      st.update(idx, val);
    }
  }

  return 0;
}