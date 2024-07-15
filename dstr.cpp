#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

struct Property {
    int ads_id;
    string prop_name;
    int completion_year;
    string monthly_rent;
    string location;
    string property_type;
    int rooms;
    int parking;
    int bathroom;
    string size;
    string furnished;

    Property() = default;

    // Constructor
    Property(int ads_id, string prop_name, int completion_year, string monthly_rent, 
             string location, string property_type, int rooms, int parking, 
             int bathroom, string size, string furnished)
        : ads_id(ads_id), prop_name(prop_name), completion_year(completion_year), monthly_rent(monthly_rent), 
          location(location), property_type(property_type), rooms(rooms), parking(parking), 
          bathroom(bathroom), size(size), furnished(furnished) {}
};

struct Node {
    Property data;
    Node* next;

    Node(const Property& property) : data(property), next(nullptr) {}
};

// To solve the bug of error reading due to comma in data
string getCSVField(istringstream& ss) {
    string field;
    if (ss.peek() == '"') {
        ss.ignore();
        getline(ss, field, '"');
        ss.ignore(); // skip the comma after the closing quote
    } else {
        getline(ss, field, ',');
    }
    return field;
}

// Function to read property data from CSV into an array
vector<Property> readPropertiesFromCSVToArray(const string& filename) {
    ifstream file(filename);
    vector<Property> properties;

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return properties;
    }

    string line;
    getline(file, line); // Removing header

    while (getline(file, line)) {
        istringstream ss(line);
        string prop_name, location, property_type, monthly_rent, size, furnished;
        string ads_id_str, completion_year_str, rooms_str, parking_str, bathroom_str;
        int ads_id, completion_year, rooms, parking, bathroom;

        ads_id_str = getCSVField(ss);
        prop_name = getCSVField(ss);
        completion_year_str = getCSVField(ss);
        monthly_rent = getCSVField(ss);
        location = getCSVField(ss);
        property_type = getCSVField(ss);
        rooms_str = getCSVField(ss);
        parking_str = getCSVField(ss);
        bathroom_str = getCSVField(ss);
        size = getCSVField(ss);
        furnished = getCSVField(ss);

        ads_id = ads_id_str.empty() ? 0 : atoi(ads_id_str.c_str());
        completion_year = completion_year_str.empty() ? 0 : atoi(completion_year_str.c_str());
        rooms = rooms_str.empty() ? 0 : atoi(rooms_str.c_str());
        parking = parking_str.empty() ? 0 : atoi(parking_str.c_str());
        bathroom = bathroom_str.empty() ? 0 : atoi(bathroom_str.c_str());
        // if empty make it -, if contains add "" to make a valid csv cell data
        if (prop_name.empty()){
            prop_name = "-";
        } else if(prop_name.find(',') != string::npos){
            prop_name = "\"" + prop_name + "\"";
        }
        if (monthly_rent.empty()) monthly_rent = "-";
        if (location.empty()) location = "-";
        if (property_type.empty()) property_type = "-";
        if (size.empty()) size = "-";
        if (furnished.empty()) furnished = "-";

        Property property(ads_id, prop_name, completion_year, monthly_rent, location, property_type, rooms, parking, bathroom, size, furnished);
        properties.push_back(property);
    }

    file.close();
    return properties;
}

// Function to convert array to linked list
Node* convertArrayToLinkedList(const vector<Property>& properties) {
    Node* head = nullptr;
    Node* tail = nullptr;

    for (const auto& property : properties) {
        Node* newNode = new Node(property);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}

Node* getTail(Node* head) {
    while (head != nullptr && head->next != nullptr) {
        head = head->next;
    }
    return head;
}


// Assist in partition function for column selection
bool compareProperties(const Property& a, const Property& b, int choice) {
    switch (choice) {
        case 1: return a.ads_id < b.ads_id;
        case 2: return a.prop_name < b.prop_name;
        case 3: return a.completion_year < b.completion_year;
        case 4: return a.monthly_rent < b.monthly_rent;
        case 5: return a.location < b.location;
        case 6: return a.property_type < b.property_type;
        case 7: return a.rooms < b.rooms;
        case 8: return a.parking < b.parking;
        case 9: return a.bathroom < b.bathroom;
        case 10: return a.size < b.size;
        case 11: return a.furnished < b.furnished;
    }
    return false;
}

Node* getMiddle(Node* head) {
    if (head == nullptr) return head;

    Node* slow = head;
    Node* fast = head->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    return slow;
}

Node* partition(Node* head, Node* end, Node** newHead, Node** newEnd, int choice) {
    // Median of three pivot selection
    Node* mid = getMiddle(head);
    swap(mid->data, end->data);

    Node* pivot = end;
    Node* prev = nullptr;
    Node* cur = head;
    Node* tail = pivot;

    while (cur != pivot) {
        if (compareProperties(cur->data, pivot->data, choice)) {
            if ((*newHead) == nullptr) {
                (*newHead) = cur;
            }
            prev = cur;  
            cur = cur->next;
        } else {
            if (prev) {
                prev->next = cur->next;
            }
            Node* tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if ((*newHead) == nullptr) {
        (*newHead) = pivot;
    }

    (*newEnd) = tail;

    return pivot;
}

Node* quickSortRecur(Node* head, Node* end, int choice, int& auxSpace) {

    if (!head || head == end) {
        return head;
    }

    Node* newHead = nullptr;
    Node* newEnd = nullptr;

    Node* pivot = partition(head, end, &newHead, &newEnd, choice);

    if (newHead != pivot) {
        Node* tmp = newHead;
        while (tmp->next != pivot) {
            tmp = tmp->next;
        }
        tmp->next = nullptr;

        newHead = quickSortRecur(newHead, tmp, choice, auxSpace);

        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd, choice, auxSpace);

    // Calculate additional space used in this call
    int localSpace = sizeof(Node) * 5; 
    auxSpace += localSpace;

    return newHead;
}

void quickSort(Node** headRef, int choice) {
    int auxSpace = sizeof(Node*) * 3;
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef), choice, auxSpace);
    cout << "Auxiliary space used: " << auxSpace << " bytes" << endl;
}

Node* merge(Node* left, Node* right, int choice, int& auxSpace) {
    Node* result = nullptr;

    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;

    switch (choice) {
        case 1: 
            if (left->data.ads_id <= right->data.ads_id) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 2:
            if (left->data.prop_name <= right->data.prop_name) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 3:
            if (left->data.completion_year <= right->data.completion_year) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 4:
            if (left->data.monthly_rent <= right->data.monthly_rent) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 5:
            if (left->data.location <= right->data.location) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 6:
            if (left->data.property_type <= right->data.property_type) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 7:
            if (left->data.rooms <= right->data.rooms) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 8:
            if (left->data.parking <= right->data.parking) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 9:
            if (left->data.bathroom <= right->data.bathroom) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 10:
            if (left->data.size <= right->data.size) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
        case 11:
            if (left->data.furnished <= right->data.furnished) {
                result = left;
                result->next = merge(left->next, right, choice, auxSpace);
            } else {
                result = right;
                result->next = merge(left, right->next, choice, auxSpace);
            }
            break;
    }

    // Calculate additional space used in this call
    int localSpace = sizeof(Node) * 3;
    auxSpace += localSpace;

    return result;
}

void mergeSortRecur(Node** headRef, int choice, int& auxSpace) {
    Node* head = *headRef;
    Node* left;
    Node* right;

    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Node* middle = getMiddle(head);
    Node* nextToMiddle = middle->next;
    middle->next = nullptr;

    mergeSortRecur(&head, choice, auxSpace);
    mergeSortRecur(&nextToMiddle, choice, auxSpace);

    *headRef = merge(head, nextToMiddle, choice, auxSpace);

}

void mergeSort(Node** headRef, int choice) {
    int auxSpace = sizeof(Node*) * 3; 
    mergeSortRecur(headRef, choice, auxSpace);
    cout << "Auxiliary space used: " << auxSpace << " bytes" << endl;
}

void sortingFunction(int sortingAlgo, Node** properties,int sortingColumn) {
    auto start = chrono::high_resolution_clock::now(); // Records start time

    sortingAlgo == 1 ? mergeSort(properties, sortingColumn) : quickSort(properties, sortingColumn);

    auto stop = chrono::high_resolution_clock::now(); // Records end time
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Time taken for sorting: " << duration.count() << " milliseconds" << endl;
    cout << "Sorting of CSV file is done!" << endl;
}

// Writing sorted data into CSV
void writePropertiesToCSV(const string& filename, Node* head) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    file << "ads_id,prop_name,completion_year,monthly_rent,location,property_type,rooms,parking,bathroom,size,furnished" << endl;

    Node* current = head;
    while (current != nullptr) {
        Property& property = current->data;
        file << property.ads_id << ","
             << property.prop_name << ","
             << property.completion_year << ","
             << property.monthly_rent << ","
             << property.location << ","
             << property.property_type << ","
             << property.rooms << ","
             << property.parking << ","
             << property.bathroom << ","
             << property.size << ","
             << property.furnished << endl;

        current = current->next;
    }

    file.close();
}

int main() {
    string inputFile = "mudah-apartment-kl-selangor mmz.csv";
    string outputFile = "output2.csv";
    vector<Property> propertiesArray = readPropertiesFromCSVToArray(inputFile);
     Node* properties = convertArrayToLinkedList(propertiesArray);
    int sortingAlgo, sortingColumn;

    cout << "Select Sorting Algorithm: " << endl << "1. Merge Sort" << endl << "2. Quick Sort" << endl;
    cin >> sortingAlgo;
    cout << "Select Sorting Column: " << endl 
         << "1. Listing ID" << endl 
         << "2. Property Name" << endl 
         << "3. Completion Year" << endl
         << "4. Monthly Rent" << endl
         << "5. Location" << endl
         << "6. Property Type" << endl
         << "7. Number of Rooms" << endl
         << "8. Number of Parking" << endl
         << "9. Number of Bathroom" << endl
         << "10. Unit Size" << endl
         << "11. Furnishing Status" << endl;
    cin >> sortingColumn;

    sortingFunction(sortingAlgo, &properties, sortingColumn);

    writePropertiesToCSV(outputFile, properties);

    return 0;
}
