/*
   You're given a line of bowling pins, each with some integral value.
   Each time you bowl, you could:
      - hit a pin individually, so that the value on that pin is appended to your score.
      - hit two pins that are immediate neighbours, so that their product is appended to your score.
   Note that you can always ignore pins you don't want to hit. Assume infinite number of balls.
   Find best score that can be achieved and delineate the strategy that leads to this score.
   Problem sourced from MIT 6.006 OCW (Introduction to Algorithms).
*/

#include <iostream>
#include <vector>

using namespace std;

const int IGNORE = 0, SINGLE = 1, PAIR_LEFT = 2;

int main() {
    int n;
    cin >> n;

    if (n <= 0) {
        cout << "Nonsense input, nonsense output." << endl;
        return 0;
    }

    vector<int> pin_values(n), memo(n), decision(n, IGNORE);
    for (int i = 0; i < n; i++) {
        cin >> pin_values[i];
        memo[i] = (i > 0) ? memo[i - 1] : 0;
        if (memo[i] + pin_values[i] > memo[i]) {
            memo[i] += pin_values[i];
            decision[i] = SINGLE;
        }
        if (i == 0) continue;
        int temp = (i > 1) ? memo[i - 2] : 0;
        if (pin_values[i] * pin_values[i - 1] + temp > memo[i]) {
            memo[i] = pin_values[i] * pin_values[i - 1] + temp;
            decision[i] = PAIR_LEFT;
        }
    }

    cout << "Best Score: " << memo[n - 1] << endl << "Strategy:" << endl;
    for (int i = n - 1; i >= 0; i--) {
        if (decision[i] == IGNORE) cout << "Don't hit " << i << endl;
        else if (decision[i] == SINGLE) cout << "Hit " << i << " individually" << endl;
        else cout << "Hit " << i-- << " and " << i << " together" << endl;
    }
    return 0;
}
