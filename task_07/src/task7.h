#pragma once
#include <vector>

class SegmentTree {
private:
    std::vector<int> tree;
    std::vector<int> data;
    int n;

    void build(int node, int start, int end);
    int query(int node, int start, int end, int l, int r);
    void update(int node, int start, int end, int idx, int val);

public:
    SegmentTree(const std::vector<int>& input_data);
    int query(int l, int r);
    void update(int idx, int val);
};