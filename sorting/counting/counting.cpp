#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Find Minimum and Maximum Value In Integer List
pair<int, int> find_min_max_pair_in_list(const vector<int>& list) {
	int min_integer = INT_MAX, max_integer = INT_MIN;
	for (const auto& item : list) {
		if (min_integer > item)
			min_integer = item;
		if (max_integer < item)
			max_integer = item;
	}
	return { min_integer, max_integer };
}

// Sort Integer List Using Counting Sort
void countingSort(vector<int>& list) {
	pair<int, int> min_max_pair = find_min_max_pair_in_list(list);
	int offset = (min_max_pair.first < 0) ? -min_max_pair.first : 0; // using offset to adjust for negatives
	int k = min_max_pair.second + offset;

	vector<int> counts(k + 1);
	for (const auto& item : list)
		counts[item + offset]++;
	for (int i = 1; i < counts.size(); i++)
		counts[i] += counts[i - 1];

	vector<int> sorted(list.size());
	for (int i = list.size() - 1; i >= 0; i--)
		sorted[--counts[list[i] + offset]] = list[i];

	list = sorted;
}

int main(int argc, char* argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: counting <input_file> <output_file>" << endl;
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

	// Sort List Using Counting Sort
	auto startTimer = high_resolution_clock::now();
	countingSort(list);
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

