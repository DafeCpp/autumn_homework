#include <iostream>
#include <vector>

#include "task2.h"

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
  int result = task2(n, m, arr);
  std::cout << result << "\n";
  return 0;
}
