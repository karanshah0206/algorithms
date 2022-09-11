#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Sort List Using Recursive Insertion Sort
void recursiveInsertionSort(vector<int>& list, int index) {
	if (index > 0) {
		recursiveInsertionSort(list, index - 1);
		int currentVal = list[index];
		int i = index - 1;
		while (i >= 0 && list[i] > currentVal) {
			list[i + 1] = list[i];
			i--;
		}
		list[i + 1] = currentVal;
	}
}

int main(int argc, char* argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: insertion_recursive <input_file> <output_file>" << endl;
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

	// Sort List Using Recursive Insertion Sort
	auto startTimer = high_resolution_clock::now();
	recursiveInsertionSort(list, list.size() - 1);
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

