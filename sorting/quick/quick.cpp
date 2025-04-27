#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;
using namespace std::chrono;

// Chooses the first item in the sublist as pivot, puts it in place,
// then recursively partitions and sorts the left and right partitions
// with respect to the index at which the pivot was finally placed.
void partitionAndSort(vector<int> &list, int lo, int hi) {
    if (lo < hi) {
        int i = lo + 1, j = hi;

        // scan the subarray from both sides; ensure no item to the left of the
        // pivot is larger than the pivot and no item to the right is smaller.
        while (true) {
            while (i <= hi && list[i] < list[lo]) i++;
            while (j > lo && list[j] > list[lo]) j--;
            if (i >= j) break;
            swap(list[i], list[j]);
        }

        // put the pivot in place
        swap(list[lo], list[j]);

        // recursively sort the left and right partitions
        partitionAndSort(list, lo, j - 1);
        partitionAndSort(list, j + 1, hi);
    }
}

// Sort List Using Quicksort
void quickSort(vector<int> &list) {
    // derive shuffle seed from hardware entropy
    random_device rd;
    default_random_engine eng(rd());
    shuffle(list.begin(), list.end(), eng);

    partitionAndSort(list, 0, list.size() - 1);
}

int main(int argc, char *argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: quick <input_file> <output_file>" << endl;
		return 1; // Error: Invalid Arguments
	}

	ifstream input(argv[1]);

	// Check Input File
	if (!input.good()) {
		cout << "Error Opening File: " << argv[1] << endl;
		return 2; // Error: Bad Input File
	}

	// Get List From Input File
	int num;
	vector<int> list;
	while (input >> num)
		list.push_back(num);
	input.close();

	// Sort List Using Quicksort
	auto startTimer = high_resolution_clock::now();
	quickSort(list);
	auto stopTimer = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stopTimer - startTimer);
	cout << duration.count() << " nanoseconds" << endl;

	// Put Sorted List In Output File
	ofstream output(argv[2]);
	if (list.size() > 0) {
		output << list[0];
		for (int i = 1; i < list.size(); i++)
			output << " " << list[i];
	}
	output.close();

	return 0; // Success
}
