#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>

using namespace std;

typedef tuple<double, double, double> item; // <value/weight, value, weight>

int main() {
    vector<item> items;
    size_t itemsCount;
    double capacity, res = 0.0;

    cin >> itemsCount >> capacity;

    while (itemsCount--) {
        double value, weight;
        cin >> value >> weight;
        items.push_back({ value/weight, value, weight });
    }

    sort(items.begin(), items.end());

    while (!items.empty()) {
        item i = items.back();
        items.pop_back();
        if (capacity >= get<2>(i)) {
            res += get<1>(i);
            capacity -= get<2>(i);
        }
        else {
            res += get<1>(i) * capacity/get<2>(i);
            break;
        }
    }

    cout << res << endl;
    return 0;
}
