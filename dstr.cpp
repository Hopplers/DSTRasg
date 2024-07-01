#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

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

// Helper function to check if a string can be converted to a double
bool isValidDouble(const string& s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+') && (s[0] != '.'))) return false;
    char * p;
    strtod(s.c_str(), &p);
    return (*p == 0);
}

// Merge function for merge sort
void merge(vector<vector<string>>& data, int left, int mid, int right, int column) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<vector<string>> L(n1);
    vector<vector<string>> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = data[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (isValidDouble(L[i][column]) && isValidDouble(R[j][column])) {
            if (stod(L[i][column]) <= stod(R[j][column])) {
                data[k] = L[i];
                i++;
            } else {
                data[k] = R[j];
                j++;
            }
        } else {
            if (L[i][column] <= R[j][column]) { // Compare strings directly
                data[k] = L[i];
                i++;
            } else {
                data[k] = R[j];
                j++;
            }
        }
        k++;
    }

    while (i < n1) {
        data[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        data[k] = R[j];
        j++;
        k++;
    }
}

// Merge sort function
void mergeSort(vector<vector<string>>& data, int left, int right, int column) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(data, left, mid, column);
        mergeSort(data, mid + 1, right, column);

        merge(data, left, mid, right, column);
    }
}

// Partition function for quicksort
int partition(vector<vector<string>>& data, int left, int right, int column) {
    string pivot = data[right][column]; // Choose the rightmost element as the pivot
    int i = left - 1;

    for (int j = left; j < right; ++j) {
        bool isJValidDouble = isValidDouble(data[j][column]);
        bool isPivotValidDouble = isValidDouble(pivot);

        if (isJValidDouble && isPivotValidDouble) {
            if (stod(data[j][column]) <= stod(pivot)) {
                i++;
                swap(data[i], data[j]);
            }
        } else if (!isJValidDouble && !isPivotValidDouble) {
            // Both are not valid doubles, compare them as strings
            if (data[j][column] <= pivot) {
                i++;
                swap(data[i], data[j]);
            }
        } else if (!isJValidDouble) {
            // Only data[j] is not a valid double
            i++;
            swap(data[i], data[j]);
        }
        // If pivot is not a valid double, it will be handled naturally
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
void sortCSVExcludingFirstRow(vector<vector<string>>& data, string sortType, int column) {
    if (data.size() > 1) { // Ensure there is more than one row (excluding header) to sort
        if (sortType == "quickSort"){
            quickSort(data, 1, data.size() - 1, column); // Start sorting from index 1 to skip the first row
        } else if (sortType == "mergeSort"){
            mergeSort(data, 1, data.size() - 1, column); // Start sorting from index 1 to skip the first row
        }
    }
}

int main() {
    string inputFile = "mudah-apartment-kl-selangor mmz.csv";
    string outputFile = "output2.csv";

    // Read the CSV file
    vector<vector<string>> data = readCSV(inputFile);

    clock_t start, end;
    double cpu_time_used;

    // Starts measure of time
    start = clock();

    // Sort the CSV data based on the second column using merge sort, excluding the first row
    sortCSVExcludingFirstRow(data, "quickSort", 1); // Manually change here first "quickSort" "mergeSort", and which column

    // Stops measure of time
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Write the sorted data back to a new CSV file
    writeCSV(outputFile, data);

    cout << "Time taken to sort: " << cpu_time_used << " seconds" << endl;

    cout << "CSV file sorted successfully!" << endl;
    return 0;
}
