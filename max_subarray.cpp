#include <iostream>
#include <vector>

using namespace std;

// get maximum subarray about a pivot (mid) and low/high bounds
vector<int> maxCrossingSubArray(vector<int>& A, int low, int mid, int high) {
    int leftSum = INT_MIN, rightSum = INT_MIN, sum = 0, maxLeft, maxRight;

    // checking values below pivot
    for (int i = mid; i >= low; i--) {
        sum += A[i];
        if (sum > leftSum) {
            leftSum = sum;
            maxLeft = i;
        }
    }

    sum = 0;

    // checking values above pivot
    for (int i = mid + 1; i <= high; i++) {
        sum += A[i];
        if (sum > rightSum) {
            rightSum = sum;
            maxRight = i;
        }
    }

    return { maxLeft, maxRight, leftSum + rightSum };
}

// get a subarray with highest summation value
// returns start/end indices (inclusive) & summation value
vector<int> maxSubArray(vector<int>& A, int low, int high) {
    // base case
    if (low == high) return { low, high, A[low] };

    int mid = (low + high) / 2;
    vector<int> lRes = maxSubArray(A, low, mid); // max subarray on left
    vector<int> hRes = maxSubArray(A, mid + 1, high); // max subarray on right
    vector<int> cRes = maxCrossingSubArray(A, low, mid, high); // max subarray about pivot (mid)

    if (lRes[2] >= hRes[2] && lRes[2] >= cRes[2]) return lRes;
    if (hRes[2] >= lRes[2] && hRes[2] >= cRes[2]) return hRes;
    return cRes;
}

int main() {
    vector<int> A = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    vector<int> r = maxSubArray(A, 0, A.size() - 1);

    cout << "start: " << r[0] << " end: " << r[1] << " value: " << r[2] << endl;
    cout << "the maximum subarray: ";
    for (int i = r[0]; i <= r[1]; i++) cout << A[i] << " ";
    cout << endl;

    return 0;
}
