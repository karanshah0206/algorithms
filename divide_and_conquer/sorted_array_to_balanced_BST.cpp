/* Given a Sorted Array, Generate A Height-Balanced BST */

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    void print() {
        if (left == nullptr && right == nullptr) return;
        cout << "," << (left == nullptr ? "null" : to_string(left->val));
        cout << "," << (right == nullptr ? "null" : to_string(right->val));
        if (left != nullptr) left->print();
        if (right != nullptr) right->print();
    }
};

TreeNode* daq(vector<int>& a, int low, int high) {
    if (low > high) return nullptr;
    if (low == high) return new TreeNode(a[low]);
    int mid = (low + high) / 2;
    return new TreeNode(a[mid], daq(a, low, mid - 1), daq(a, mid + 1, high));
}

int main() {
    vector<int> a = { -10, -3, 0, 5, 9 };
    TreeNode* heightBalancedBST = daq(a, 0, a.size() - 1);
    cout << heightBalancedBST->val;
    heightBalancedBST->print();
    cout << endl;
    return 0;
}
