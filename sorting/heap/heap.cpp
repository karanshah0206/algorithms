#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Restore Heap Order Top-Down From Given Index
void sink(vector<int> &list, int size, int i) {
    while (i < size / 2) {
        // Identify The Larger Child
        int j = 2 * i + 1;
        if (j + 1 < size && list[j] < list[j + 1])
            j++;

        // Swap With Parent If Child Is Larger
        if (list[i] < list[j]) {
            int temp = list[i];
            list[i] = list[j];
            list[j] = temp;
            i = j;
        } else break;
    }
}

// Sort List Using Heap Sort
void heapSort(vector<int> &list) {
    // Heapify Array In Linear Time
    for (int i = list.size() / 2 - 1; i >= 0; i--)
        sink(list, list.size(), i);

    for (int i = list.size() - 1; i > 0; i--) {
        // Place Next Largest Item In Position
        int largest = list[0];
        list[0] = list[i];
        list[i] = largest;

        // Restore Heap Order
        sink(list, i, 0);
    }
}

int main(int argc, char *argv[]) {
    // Check Arguments
    if (argc < 3) {
        cout << "Invalid Arguments." << "Usage: heap <input_file> <output_file>" << endl;
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

    // Sort List Using Heap Sort
    auto startTimer = high_resolution_clock::now();
    heapSort(list);
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
