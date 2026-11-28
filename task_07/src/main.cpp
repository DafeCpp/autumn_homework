#include <iostream>
#include <vector>

#include "rmq.hpp"

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int n, q;
  if (!(std::cin >> n >> q)) {
    return 0;
  }

  std::vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> a[i];
  }

  SegmentTree st(a);

  for (int i = 0; i < q; ++i) {
    int type;
    std::cin >> type;
    if (type == 1) {
      int l, r;
      std::cin >> l >> r;
      std::cout << st.Query(l - 1, r - 1) << "\n";
    } else if (type == 2) {
      int idx, val;
      std::cin >> idx >> val;
      st.Update(idx - 1, val);
    }
  }

  return 0;
}