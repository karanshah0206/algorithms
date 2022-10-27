#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Merge Two Halfs Of List
void merge(vector<int>& list, int l, int m, int r) {
	int n1 = m - l + 1, n2 = r - m;
	int L[n1], R[n2];

	for (int i = 0; i < n1; i++) L[i] = list[l + i];
	for (int i = 0; i < n2; i++) R[i] = list[m + 1 + i];

	int i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			list[k] = L[i];
			i++;
		}
		else {
			list[k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		list[k] = L[i];
		i++; k++;
	}

	while (j < n2) {
		list[k] = R[j];
		j++; k++;
	}
}

// Sort List Using Merge Sort
void mergeSort(vector<int>& list) {
	int n = list.size() - 1;
	for (int i = 1; i <= n; i *= 2)
		for (int j = 0; j < n; j += 2 * i) {
			int mid = ((j + i - 1) < n) ? (j + i - 1) : n;
			int rightEnd = ((j + 2 * i - 1) < n) ? (j + 2 * i - 1) : n;
			merge(list, j, mid, rightEnd);
		}
}

int main(int argc, char* argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: merge <input_file> <output_file>" << endl;
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

	// Sort List Using Merge Sort
	auto startTimer = high_resolution_clock::now();
	mergeSort(list);
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

