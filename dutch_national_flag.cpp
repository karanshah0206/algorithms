// Turns out we don't need 3-way partitioning to solve the Dutch national flag problem.
// Focusing on 0 and 2 suffices, we can ignore the 1s and let fate (the algorithm, implicitly) take care of them.

#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    while (cin >> n) {
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        int swap_0_to = 0, swap_2_to = n - 1;
        for (int i = 0; i < n; i++) {
            if (a[i] == 0 && i >= swap_0_to) {
                a[i] = a[swap_0_to];
                a[swap_0_to++] = 0;
                i--;
            } else if (a[i] == 2 && i <= swap_2_to) {
                a[i] = a[swap_2_to];
                a[swap_2_to--] = 2;
                i--;
            }
        }

        for (int i = 0; i < n; i++) cout << a[i] << " ";
        cout << "\n";
    }
}
