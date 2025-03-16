/*
   Input is an integer array representing coins of specified values.
   Player can take either the left-most or right-most coin at each turn.
   Player 1 goes first, Player 2 goes next; the two alternate turns. Both play optimally.
   Goal is to maximise the net value of coins Player 1 collects.
   Problem sourced from MIT 6.006 OCW (Introduction to Algorithms).
*/

#include <iostream>
#include <vector>

using namespace std;

int player_1(vector<int> &coins, vector<vector<pair<int, int>>> &memo, int i, int j);
int player_2(vector<int> &coins, vector<vector<pair<int, int>>> &memo, int i, int j);

int player_1(vector<int> &coins, vector<vector<pair<int, int>>> &memo, int i, int j) {
    if (i < 0 || j >= coins.size() || i > j) return 0;
    if (memo[i][j].first == -1) {
        if (i == j) memo[i][j].first = coins[i];
        else memo[i][j].first = max(player_2(coins, memo, i + 1, j) + coins[i], player_2(coins, memo, i, j - 1) + coins[j]);
    }
    return memo[i][j].first;
}

int player_2(vector<int> &coins, vector<vector<pair<int, int>>> &memo, int i, int j) {
    if (i < 0 || j >= coins.size() || i > j) return 0;
    if (memo[i][j].second == -1) {
        if (i == j) memo[i][j].second = 0;
        else memo[i][j].second = min(player_1(coins, memo, i + 1, j), player_1(coins, memo, i, j - 1));
    }
    return memo[i][j].second;
}

vector<pair<int, int>> backtrack(vector<int> &coins, vector<vector<pair<int, int>>> &memo) {
    vector<pair<int, int>> result;
    int i = 0, j = coins.size() - 1, move_counter = 0;
    while (i <= j)
        if (++move_counter % 2 == 1) {
            if (i == j || memo[i][j].first == coins[i] + memo[i+1][j].second) result.push_back({ 1, coins[i++] });
            else result.push_back({ 1, coins[j--] });
        } else {
            if (i == j || memo[i][j].second == memo[i+1][j].first) result.push_back({ 2, coins[i++] });
            else result.push_back({ 2, coins[j--] });
        }
    return result;
}

int main() {
    int n;
    cin >> n;

    vector<int> coins(n);
    for (int i = 0; i < n; i++) cin >> coins[i];

    vector<vector<pair<int, int>>> memo(n, vector<pair<int, int>>(n, { -1, -1 }));
    cout << "Score: " << player_1(coins, memo, 0, coins.size() - 1) << endl << "Moves:" << endl;
    for (auto &[player, coin] : backtrack(coins, memo)) cout << "Player " << player << ": " << coin << endl;

    return 0;
}
