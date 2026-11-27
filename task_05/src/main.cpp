#include <iostream>
#include <vector>

#include "task5.h"
using namespace std;

int main() {
  int n, m, d;
  cin >> n >> m >> d;

  vector<int> arr(m * 3);
  for (int i = 0; i < m * 3; ++i) {
    cin >> arr[i];
  }

  vector<string> result = task5(n, m, d, arr);

  for (auto s : result) {
    cout << s << "\n";
  }

  return 0;
}