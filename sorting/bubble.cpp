#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Sort List Using Bubble Sort
void bubbleSort(vector<int>& list) {
	bool swapped;
	do {
		swapped = false;
		for (int i = 1; i < list.size(); i++) {
			if (list[i-1] > list[i]) {
				int temp = list[i-1];
				list[i-1] = list[i];
				list[i] = temp;
				swapped = true;
			}
		}
	} while (swapped);
}

int main(int argc, char* argv[]) {
	// Check Arguments
	if (argc < 3) {
		cout << "Invalid Arguments." << "Usage: bubble <input_file> <output_file>" << endl;
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
	bubbleSort(list);

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

