/* Determine the count of values in a 1D container within a given range. */

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = { 5, 7, 3, 9, 11, 22, 1, 6, 12, 4, 2 };
    std::sort(v.begin(), v.end());

    int range_begin = 7, range_end = 15; // count items in range [7, 15]

    std::vector<int>::iterator l = std::lower_bound(v.begin(), v.end(), range_begin);
    std::vector<int>::iterator r = std::upper_bound(v.begin(), v.end(), range_end);

    int range_count = r - l;

    std::cout << "Count of items in range: " << range_count << "\n";
    std::cout << "List of items in range: ";
    while (l < r) {
        std::cout << *l++ << " ";
    }
    std::cout << "\n";
}
