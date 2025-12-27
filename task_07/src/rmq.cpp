#include "rmq.h"

#include <algorithm>
#include <climits>

RMQ::RMQ(const std::vector<int>& arr) : size_(arr.size()), arr_(arr) {
  seg_tree_.resize(4 * size_);
  Build(1, 0, size_ - 1);
}

void RMQ::Build(int node, int start, int end) {
  if (start == end) {
    seg_tree_[node] = arr_[start];
  } else {
    int mid = (start + end) / 2;
    Build(node * 2, start, mid);
    Build(node * 2 + 1, mid + 1, end);
    seg_tree_[node] = std::min(seg_tree_[node * 2], seg_tree_[node * 2 + 1]);
  }
}

void RMQ::Update(int index, int value) {
  UpdateTree(1, 0, size_ - 1, index, value);
}

void RMQ::UpdateTree(int node, int start, int end, int idx, int value) {
  if (start == end) {
    arr_[idx] = value;
    seg_tree_[node] = value;
  } else {
    int mid = (start + end) / 2;
    if (start <= idx && idx <= mid) {
      UpdateTree(node * 2, start, mid, idx, value);
    } else {
      UpdateTree(node * 2 + 1, mid + 1, end, idx, value);
    }
    seg_tree_[node] = std::min(seg_tree_[node * 2], seg_tree_[node * 2 + 1]);
  }
}

int RMQ::Query(int left, int right) const {
  return QueryTree(1, 0, size_ - 1, left, right);
}

int RMQ::QueryTree(int node, int start, int end, int left, int right) const {
  if (right < start || end < left) {
    return INT_MAX;
  }
  if (left <= start && end <= right) {
    return seg_tree_[node];
  }
  int mid = (start + end) / 2;
  int left_min = QueryTree(node * 2, start, mid, left, right);
  int right_min = QueryTree(node * 2 + 1, mid + 1, end, left, right);
  return std::min(left_min, right_min);
}