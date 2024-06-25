/* Given a set of weighted intervals, find maximum weight that can be achieved with a non-conflicting schedule. */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<pair<size_t, size_t>, size_t> job; // { { startTime, endTime }, weight }

struct Compare {
    bool operator()(const job& a, const job& b) {
        return a.first.second < b.first.second;
    }
};

size_t n;
vector<size_t> dp;
vector<job> times;

int prevCompatible(size_t i) {
    for (int j = i - 1; j >= 0; j--)
        if (times[i].first.first >= times[j].first.second)
            return j;
    return -1;
}

int main() {
    cin >> n;
    dp = vector<size_t>(n, 0);
    times = vector<job>(n);
    for (size_t i = 0; i < n; i++) cin >> times[i].first.first >> times[i].first.second >> times[i].second;
    sort(times.begin(), times.end(), Compare());
    dp[0] = times[0].second;
    for (size_t i = 1; i < n; i++) {
        size_t pc = prevCompatible(i);
        if (pc == -1) dp[i] = max(times[i].second, dp[i - 1]);
        else dp[i] = max(times[i].second + dp[pc], dp[i - 1]);
    }
    cout << dp[n - 1] << endl;
    return 0;
}
