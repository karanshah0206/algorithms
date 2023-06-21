#include <iostream>
#include <vector>

using namespace std;

const int NINF = -1e8;

vector<int> across(vector<int>& a, int low, int mid, int high) {
    int left_running_sum = 0, left_max_sum = NINF, left_index = mid;
    for (int i = left_index; i >= low; i--) {
        left_running_sum += a[i];
        if (left_running_sum > left_max_sum) {
            left_max_sum = left_running_sum;
            left_index = i;
        }
    }
    int right_running_sum = 0, right_max_sum = NINF, right_index = mid + 1;
    for (int i = right_index; i <= high; i++) {
        right_running_sum += a[i];
        if (right_running_sum > right_max_sum) {
            right_max_sum = right_running_sum;
            right_index = i;
        }
    }
    return { left_index, right_index, left_max_sum + right_max_sum };
}

vector<int> daq(vector<int>& a, int low, int high) {
    if (low == high) return { low, high, a[low] };
    int mid = (low + high) / 2;
    vector<int> left = daq(a, low, mid);
    vector<int> right = daq(a, mid + 1, high);
    vector<int> acr = across(a, low, mid, high);
    if (left[2] >= right[2] && left[2] >= acr[2]) return left;
    if (right[2] >= left[2] && right[2] >= acr[2]) return right;
    return acr;
}

int main() {
    vector<int> a = { 2, -1, 4, -5, -7 };
    vector<int> res = daq(a, 0, a.size() - 1);
    cout << res[0] << " " << res[1] << " " << res[2] << endl;
    return 0;
}
