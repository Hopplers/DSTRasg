#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// Function to split a line into tokens
vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to read the CSV file
vector<vector<string>> readCSV(const string& filename) {
    ifstream file(filename);
    vector<vector<string>> data;
    string line;
    if (getline(file, line)) {
        data.push_back(split(line, ',')); // Store the header row
    }
    while (getline(file, line)) {
        data.push_back(split(line, ','));
    }
    return data;
}

// Function to write the sorted data back to a CSV file
void writeCSV(const string& filename, const vector<vector<string>>& data) {
    ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            file << data[i][j];
            if (j < data[i].size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
}

// Partition function for quicksort
int partition(vector<vector<string>>& data, int left, int right, int column) {
    string pivot = data[right][column]; // Choose the rightmost element as the pivot
    int i = left - 1;

    for (int j = left; j < right; ++j) {
        if (data[j][column] <= pivot) {
            i++;
            swap(data[i], data[j]);
        }
    }

    swap(data[i + 1], data[right]);
    return i + 1;
}

// Quicksort function
void quickSort(vector<vector<string>>& data, int left, int right, int column) {
    if (left < right) {
        int pi = partition(data, left, right, column);

        quickSort(data, left, pi - 1, column);
        quickSort(data, pi + 1, right, column);
    }
}

// Function to sort the CSV data excluding the first row (header)
void sortCSVExcludingFirstRow(vector<vector<string>>& data, int column) {
    if (data.size() > 1) { // Ensure there is more than one row (excluding header) to sort
        quickSort(data, 1, data.size() - 1, column); // Start sorting from index 1 to skip the first row
    }
}

int main() {
    string inputFile = "output.csv";
    string outputFile = "output2.csv";

    // Read the CSV file
    vector<vector<string>> data = readCSV(inputFile);

    // Sort the CSV data based on the specified column using quicksort, excluding the first row
    sortCSVExcludingFirstRow(data, 0); // Change '6' to the desired column index for sorting

    // Write the sorted data back to a new CSV file
    writeCSV(outputFile, data);

    cout << "CSV file sorted successfully!" << endl;
    return 0;
}
