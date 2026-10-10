#include <stack>
#include <vector>

class AdditionalRoads {
 public:
  int amount{0};

  AdditionalRoads(const std::vector<std::vector<int>>& g) {
    graf = g;
    on_stack.assign(graf.size(), false);
    tin.assign(graf.size(), 2e9);
    low.assign(graf.size(), 2e9);
    components_id.assign(graf.size(), -1);

    amount = GetAmountNewRoads();
  };

 private:
  int timer{-1};
  std::vector<std::vector<int>> graf;
  std::vector<bool> on_stack;
  std::vector<int> tin, low;
  std::stack<int> stack;
  std::vector<std::vector<int>> components;
  std::vector<int> components_id;
  std::vector<bool> has_in;
  std::vector<bool> has_out;

  void DFS(int u);
  int GetAmountNewRoads();
  void GetSCC();
};
