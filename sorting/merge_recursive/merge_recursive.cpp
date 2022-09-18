#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Sort Elements In Source List And Overwrite Sorted Elements In Targer List
void merge(vector<int>& sourceList, size_t leftIndex, size_t midIndex, size_t rightIndex, vector<int>& targetList) {
	size_t leftListIndex = leftIndex, rightListIndex = midIndex;
	for (size_t i = leftIndex; i < rightIndex; i++)
		if (leftListIndex < midIndex && (rightListIndex >= rightIndex || sourceList[leftListIndex] <= sourceList[rightListIndex])) {
			targetList[i] = sourceList[leftListIndex];
			leftListIndex++;
		}
		else {
			targetList[i] = sourceList[rightListIndex];
			rightListIndex++;
		}
}

// Recursive Split List Until Atomic, Sort & Merge In A Duplicate List, Then Overwrite Original List
void mergeSplit(vector<int>& duplicateList, size_t leftIndex, size_t rightIndex, vector<int>& list) {
	if (rightIndex - leftIndex > 1) {
		size_t midIndex = (leftIndex + rightIndex) / 2;
		mergeSplit(list, leftIndex, midIndex, duplicateList);
		mergeSplit(list, midIndex, rightIndex, duplicateList);
		merge(duplicateList, leftIndex, midIndex, rightIndex, list);
	}
}

// Sort List Using Recursive Merge Sort
void recursiveMergeSort(vector<int>& list) {
	vector<int> duplicateList = list;
	mergeSplit(duplicateList, 0, list.size(), list);
}

int main(int argc, char* argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: merge_recursive <input_file> <output_file>" << endl;
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

	// Sort List Using Bubble Sort
	auto startTimer = high_resolution_clock::now();
	recursiveMergeSort(list);
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

