#include <iostream>
#include <vector>

using namespace std;

int lcs(vector<vector<int>> &memo, const string &s1, const string &s2, int i1 = 0, int i2 = 0) {
    if (memo[i1][i2] == -1) {
        if (i1 == s1.size() || i2 == s2.size()) memo[i1][i2] = 0;
        else if (s1[i1] == s2[i2]) memo[i1][i2] = 1 + lcs(memo, s1, s2, i1 + 1, i2 + 1);
        else memo[i1][i2] = max(lcs(memo, s1, s2, i1, i2 + 1), lcs(memo, s1, s2, i1 + 1, i2));
    }
    return memo[i1][i2];
}

string backtrack(vector<vector<int>> &memo, const string &s1, const string &s2) {
    string result;
    for (int i1 = 0, i2 = 0; i1 < s1.size() && i2 < s2.size(); i1++, i2++)
        if (s1[i1] == s2[i2]) result += s1[i1];
        else if (memo[i1 + 1][i2] == memo[i1][i2]) i2--;
        else i1--;
    return result;
}

int main() {
    string s1, s2;
    cin >> s1 >> s2;

    vector<vector<int>> memo(s1.size() + 1, vector<int>(s2.size() + 1, -1));

    cout << "LCS Size:\t" << lcs(memo, s1, s2) << endl;
    cout << "LCS String:\t" << backtrack(memo, s1, s2) << endl;

    cout << "Visualising memo (using ~ to represent end of string):" << endl;
    for (int j = 0; j < memo[0].size(); j++) cout << "\t" << (j < s2.size() ? s2[j] : '~');
    cout << endl;
    for (int i = 0; i < memo.size(); i++) {
        cout << (i < s1.size() ? s1[i] : '~');
        for (int j = 0; j < memo[0].size(); j++) cout << "\t" << memo[i][j];
        cout << endl;
    }

    return 0;
}
