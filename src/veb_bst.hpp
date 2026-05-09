#pragma once
#include <vector>
#include <cmath>

struct veb_best {
    std::vector<int> flat;
    int n;
    // u is the sorted input array
    veb_best(const std::vector<int>& u) {
        n = static_cast<int>(u.size());
        flat.resize(n);
        const std::vector<int>& sorted = u;
        std::vector<int> heap(n + 1);
        sorted_to_heap(sorted, heap, 1, 0, n - 1);
        int idx = 0;
        veb_layout(heap, 1, tree_height(n), idx);
    }

    int tree_height(int num_nodes) {
        if (num_nodes <= 0) return 0;
        return (int)std::ceil(std::log2(num_nodes + 1));
    }

    void sorted_to_heap(const std::vector<int>& sorted, std::vector<int>& heap,
                        int node, int start, int end) {
        if (start > end || node > n) return;
        int mid = start + (end - start) / 2;
        heap[node] = sorted[mid];
        sorted_to_heap(sorted, heap, 2 * node,     start,   mid - 1);
        sorted_to_heap(sorted, heap, 2 * node + 1, mid + 1, end);
    }

    void veb_layout(const std::vector<int>& heap, int root, int h, int& idx) {
        if (h == 0 || root > n) return;
        if (h == 1) {
            flat[idx++] = heap[root];
            return;
        }
        int h_top = h / 2;
        int h_bot = h - h_top;
        int num_bot_trees = (1 << h_top);
        veb_layout(heap, root, h_top, idx);
        int base = root * (1 << h_top);
        for (int i = 0; i < num_bot_trees; i++) {
            veb_layout(heap, base + i, h_bot, idx);
        }
    }

    int find_root(int pos, int h) {
        while (h > 1) h = h / 2;
        return pos;
    }

    int subtree_size(int h) {
        return (1 << h) - 1;
    }

    bool search_veb(int pos, int h, int x) {
        if (h == 0) return false;
        if (h == 1) return flat[pos] == x;

        int h_top = h / 2;
        int h_bot = h - h_top;
        int top_size = subtree_size(h_top);
        int bot_size = subtree_size(h_bot);
        int num_bot_trees = (1 << h_top);

        int root_pos = find_root(pos, h_top);
        int root_val = flat[root_pos];

        if (x == root_val) return true;

        int bot_base = pos + top_size;

        if (x < root_val) {
            return search_veb(bot_base, h_bot, x);
        }

        int cur_pos = pos;
        int cur_h   = h_top;

        while (cur_h > 1) {
            int ch_top  = cur_h / 2;
            int ch_bot  = cur_h - ch_top;
            int ch_top_size = subtree_size(ch_top);
            int ch_bot_size = subtree_size(ch_bot);
            int rp = find_root(cur_pos, ch_top);
            int rv = flat[rp];
            int ch_bot_base = cur_pos + ch_top_size;
            if (x < rv) {
                cur_pos = ch_bot_base;
            } else if (x > rv) {
                cur_pos = ch_bot_base + ch_bot_size;
            } else {
                return true;
            }
            cur_h = ch_bot;
        }

        if (flat[cur_pos] == x) return true;

        int leaf_idx = cur_pos - (pos + top_size);
        if (leaf_idx < 0 || leaf_idx >= num_bot_trees) return false;

        return search_veb(bot_base + leaf_idx * bot_size, h_bot, x);
    }

    bool search(int x) {
        return search_veb(0, tree_height(n), x);
    }
};