#include <iostream>
#include <vector>

using namespace std;

// params: max capacity, values, weights
size_t knapsack(size_t c, vector<size_t>& v, vector<size_t>& w) {
    size_t n = v.size();
    vector<vector<size_t>> dp(n + 1, vector<size_t>(c + 1));
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= c; j++) {
            if (w[i-1] > j) dp[i][j] = dp[i-1][j];
            else if (v[i-1] + dp[i-1][j-w[i-1]] >= dp[i-1][j]) dp[i][j] = v[i-1] + dp[i-1][j-w[i-1]];
            else dp[i][j] = dp[i-1][j];
        }
    }
    return dp[n][c];
}

int main() {
    vector<size_t> v, w;
    size_t n, c;
    cin >> n >> c;
    while (n-- > 0) {
        size_t a, b;
        cin >> a >> b;
        v.push_back(a);
        w.push_back(b);
    }
    cout << knapsack(c, v, w) << endl;
    return 0;
}
