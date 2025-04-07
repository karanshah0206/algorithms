#include <iostream>
#include <vector>

class DisjointSet {
private:
    std::vector<int> root, child_count;

    int get_root(int i) {
        while (i != root[i]) {
            root[i] = root[root[i]]; // path compression optimisation to shrink tree height
            i = root[i];
        }
        return i;
    }

public:
    DisjointSet(int n) {
        root = child_count = std::vector<int>(n, 1);
        for (int i = 0; i < n; i++) root[i] = i;
    }

    void join(int i, int j) {
        int i_root = get_root(i), j_root = get_root(j);

        // Always attach root of smaller tree to root of larger tree
        // to keep the overall tree height as minimal as possible.
        if (child_count[i_root] >= child_count[j_root]) {
            root[j_root] = i_root;
            child_count[i_root] += child_count[j_root];
        } else {
            root[i_root] = j_root;
            child_count[j_root] += child_count[i_root];
        }
    }

    bool are_joint(int i, int j) {
        return get_root(i) == get_root(j);
    }
};

int main() {
    // Testing
    DisjointSet disjoint_set(10);
    disjoint_set.join(4, 3);
    disjoint_set.join(3, 8);
    disjoint_set.join(6, 5);
    disjoint_set.join(9, 4);
    disjoint_set.join(2, 1);
    std::cout << disjoint_set.are_joint(0, 7) << std::endl; // false
    std::cout << disjoint_set.are_joint(8, 9) << std::endl; // true
    disjoint_set.join(5, 0);
    disjoint_set.join(7, 2);
    disjoint_set.join(6, 1);
    disjoint_set.join(7, 3);
    std::cout << disjoint_set.are_joint(7, 0) << std::endl; // true

    return 0;
}
