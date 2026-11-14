#include <iostream>
#include <vector>

#include "task1.h"

int main() {
  int n, m;
  std::cin >> n >> m;
  std::vector<int> arr;
  for (int i = 0; i < m; i++) {
    int a, b;
    std::cin >> a >> b;
    arr.push_back(a);
    arr.push_back(b);
  }
  auto result = task1(n, m, arr);
  for (int i : result) {
    std::cout << i << " ";
  }
  return 0;
}
