#ifndef RMQ_H
#define RMQ_H

#include <vector>

class RMQ {
 public:
  RMQ(const std::vector<int>& arr);
  void Update(int index, int value);
  int Query(int left, int right) const;

 private:
  void Build(int node, int start, int end);
  void UpdateTree(int node, int start, int end, int idx, int value);
  int QueryTree(int node, int start, int end, int left, int right) const;
  
  int size_;
  std::vector<int> arr_;
  std::vector<int> seg_tree_;
};

#endif