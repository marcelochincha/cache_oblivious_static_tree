#pragma once
#include <vector>

struct flat_bst {
    std::vector<int> flat;
    int n;

    //u is the sorted input array
    flat_bst(const std::vector<int>& u) {
        n = static_cast<int>(u.size());
        flat.resize(n);
        const std::vector<int>& sorted = u;
        build_flat_bst(sorted, 0, n - 1, 0);
    }

    void build_flat_bst(const std::vector<int>& sorted, int start, int end, int idx) {
        if (start > end || idx >= n) return;
        int mid = start + (end - start) / 2;
        flat[idx] = sorted[mid];
        build_flat_bst(sorted, start, mid - 1, 2 * idx + 1);
        build_flat_bst(sorted, mid + 1, end, 2 * idx + 2);
    }

    bool search(int x) {
        int idx = 0;
        while (idx < n) {
            if (flat[idx] == x) return true;
            else if (x < flat[idx]) idx = 2 * idx + 1; // left child
            else idx = 2 * idx + 2; // right child
        }
        return false;
    }
};