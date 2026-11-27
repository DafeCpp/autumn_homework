#include <iostream>
#include <string>
#include <vector>

#include "task3.h"

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
  auto result = task3(n, m, arr);
  for (std::string i : result) {
    std::cout << i << "\n";
  }
  return 0;
}
