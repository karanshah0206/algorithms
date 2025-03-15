#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

int lis(vector<int> &values, vector<int> &memo, vector<int> &child, int i) {
    if (i >= values.size()) return 0;
    if (memo[i] <= 0) {
        memo[i] = 1;
        for (int j = i + 1; j < values.size(); j++) {
            int new_lis_size = 1 + lis(values, memo, child, j);
            if (values[j] > values[i] && new_lis_size > memo[i]) {
                memo[i] = new_lis_size;
                child[i] = j;
            }
        }
    }
    return memo[i];
}

vector<int> backtrack(vector<int> &values, vector<int> &memo, vector<int> &child) {
    vector<int> result;
    int index = 0;
    for (int i = 1; i < values.size(); i++) if (memo[i] > memo[index]) index = i;
    while (index != -1) {
        result.push_back(values[index]);
        index = child[index];
    }
    return result;
}

int main() {
    int n;
    cin >> n;
    assert(n > 0);

    vector<int> values(n), memo(n), child(n, -1);
    for (int i = 0; i < n; i++) cin >> values[i];

    for (int i = 0; i < n; i++) lis(values, memo, child, i);
    vector<int> lis_result = backtrack(values, memo, child);

    cout << "LIS size: " << lis_result.size() << endl << "LIS:";
    for (int value : lis_result) cout << " " << value;
    cout << endl;

    return 0;
}
