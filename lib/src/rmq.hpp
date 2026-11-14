#pragma once

#include <algorithm>
#include <limits>
#include <vector>

class SegmentTree {
 public:
  explicit SegmentTree(const std::vector<int>& a) {
    n_ = static_cast<int>(a.size());
    tree_.assign(4 * n_, std::numeric_limits<int>::max());
    if (n_ > 0) {
      Build(1, 0, n_ - 1, a);
    }
  }

  void Update(int idx, int val) {
    if (n_ == 0) return;
    Update(1, 0, n_ - 1, idx, val);
  }

  int Query(int l, int r) const {
    if (n_ == 0 || l > r) return std::numeric_limits<int>::max();
    return Query(1, 0, n_ - 1, l, r);
  }

 private:
  int n_;
  std::vector<int> tree_;

  void Build(int node, int start, int end, const std::vector<int>& a) {
    if (start == end) {
      tree_[node] = a[start];
      return;
    }
    int mid = start + (end - start) / 2;
    Build(2 * node, start, mid, a);
    Build(2 * node + 1, mid + 1, end, a);
    tree_[node] = std::min(tree_[2 * node], tree_[2 * node + 1]);
  }

  void Update(int node, int start, int end, int idx, int val) {
    if (start == end) {
      tree_[node] = val;
      return;
    }
    int mid = start + (end - start) / 2;
    if (idx <= mid) {
      Update(2 * node, start, mid, idx, val);
    } else {
      Update(2 * node + 1, mid + 1, end, idx, val);
    }
    tree_[node] = std::min(tree_[2 * node], tree_[2 * node + 1]);
  }

  int Query(int node, int start, int end, int l, int r) const {
    if (r < start || end < l) {
      return std::numeric_limits<int>::max();
    }
    if (l <= start && end <= r) {
      return tree_[node];
    }
    int mid = start + (end - start) / 2;
    int p1 = Query(2 * node, start, mid, l, r);
    int p2 = Query(2 * node + 1, mid + 1, end, l, r);
    return std::min(p1, p2);
  }
};