#include "task7.h"

#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

SegmentTree::SegmentTree(const vector<int>& input_data) {
  n = input_data.size() - 1;
  data = input_data;
  tree.assign(4 * n + 1, 0);
  if (n > 0) {
    build(1, 1, n);
  }
}

void SegmentTree::build(int node, int start, int end) {
  if (start == end) {
    tree[node] = data[start];
  } else {
    int mid = start + (end - start) / 2;
    int left_child = 2 * node;
    int right_child = 2 * node + 1;

    build(left_child, start, mid);
    build(right_child, mid + 1, end);

    tree[node] = min(tree[left_child], tree[right_child]);
  }
}

int SegmentTree::query(int node, int start, int end, int l, int r) {
  if (r < start || end < l) {
    return INF;
  }

  if (l <= start && end <= r) {
    return tree[node];
  }

  int mid = start + (end - start) / 2;
  int left_child = 2 * node;
  int right_child = 2 * node + 1;

  int p1 = query(left_child, start, mid, l, r);
  int p2 = query(right_child, mid + 1, end, l, r);

  return min(p1, p2);
}

void SegmentTree::update(int node, int start, int end, int idx, int val) {
  if (start == end) {
    data[idx] = val;
    tree[node] = val;
  } else {
    int mid = start + (end - start) / 2;
    int left_child = 2 * node;
    int right_child = 2 * node + 1;

    if (idx <= mid) {
      update(left_child, start, mid, idx, val);
    } else {
      update(right_child, mid + 1, end, idx, val);
    }

    tree[node] = min(tree[left_child], tree[right_child]);
  }
}

int SegmentTree::query(int l, int r) { return query(1, 1, n, l, r); }

void SegmentTree::update(int idx, int val) { update(1, 1, n, idx, val); }