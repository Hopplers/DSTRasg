#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

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

Node* readPropertiesFromCSV(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return nullptr;
    }

    string line;
    getline(file, line); // Removing header

    Node* head = nullptr;
    Node* tail = nullptr;

    while (getline(file, line)) {
        istringstream ss(line);
        string prop_name, location, property_type, monthly_rent, size, furnished;
        string ads_id_str, completion_year_str, rooms_str, parking_str, bathroom_str;
        int ads_id, completion_year, rooms, parking, bathroom;

        getline(ss, ads_id_str, ',');
        getline(ss, prop_name, ',');
        getline(ss, completion_year_str, ',');
        getline(ss, monthly_rent, ',');
        getline(ss, location, ',');
        getline(ss, property_type, ',');
        getline(ss, rooms_str, ',');
        getline(ss, parking_str, ',');
        getline(ss, bathroom_str, ',');
        getline(ss, size, ',');
        getline(ss, furnished, ',');

        ads_id = ads_id_str.empty() ? 0 : atoi(ads_id_str.c_str());
        completion_year = completion_year_str.empty() ? 0 : atoi(completion_year_str.c_str());
        rooms = rooms_str.empty() ? 0 : atoi(rooms_str.c_str());
        parking = parking_str.empty() ? 0 : atoi(parking_str.c_str());
        bathroom = bathroom_str.empty() ? 0 : atoi(bathroom_str.c_str());
        if (prop_name.empty()) prop_name = "-";
        if (monthly_rent.empty()) monthly_rent = "-";
        if (location.empty()) location = "-";
        if (property_type.empty()) property_type = "-";
        if (size.empty()) size = "-";
        if (furnished.empty()) furnished = "-";

        Property property(ads_id, prop_name, completion_year, monthly_rent, location, property_type, rooms, parking, bathroom, size, furnished);
        Node* newNode = new Node(property);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    file.close();
    return head;
}

Node* getTail(Node* head) {
    while (head != nullptr && head->next != nullptr) {
        head = head->next;
    }
    return head;
}

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


Node* quickSortRecur(Node* head, Node* end, int choice) {
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

        newHead = quickSortRecur(newHead, tmp, choice);

        tmp = getTail(newHead);
        tmp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd, choice);

    return newHead;
}

void quickSort(Node** headRef, int choice) {
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef), choice);
}

void printProperties(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        cout << current->data.ads_id << ", "
             << current->data.prop_name << ", "
             << current->data.completion_year << ", "
             << current->data.monthly_rent << ", "
             << current->data.location << ", "
             << current->data.property_type << ", "
             << current->data.rooms << ", "
             << current->data.parking << ", "
             << current->data.bathroom << ", "
             << current->data.size << ", "
             << current->data.furnished << endl;
        current = current->next;
    }
}

Node* merge(Node* left, Node* right, int choice) {
    Node* result = nullptr;

    if (left == nullptr)
        return right;
    if (right == nullptr)
        return left;

    switch (choice) {
        case 1: 
            if (left->data.ads_id <= right->data.ads_id) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 2:
            if (left->data.prop_name <= right->data.prop_name) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 3:
            if (left->data.completion_year <= right->data.completion_year) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 4:
            if (left->data.monthly_rent <= right->data.monthly_rent) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 5:
            if (left->data.location <= right->data.location) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 6:
            if (left->data.property_type <= right->data.property_type) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 7:
            if (left->data.rooms <= right->data.rooms) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 8:
            if (left->data.parking <= right->data.parking) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 9:
            if (left->data.bathroom <= right->data.bathroom) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 10:
            if (left->data.size <= right->data.size) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
        case 11:
            if (left->data.furnished <= right->data.furnished) {
                result = left;
                result->next = merge(left->next, right, choice);
            } else {
                result = right;
                result->next = merge(left, right->next, choice);
            }
            break;
    }

    return result;
}

void mergeSort(Node** headRef, int choice) {
    Node* head = *headRef;
    Node* left;
    Node* right;

    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Node* middle = getMiddle(head);
    Node* nextToMiddle = middle->next;
    middle->next = nullptr;

    mergeSort(&head, choice);
    mergeSort(&nextToMiddle, choice);

    *headRef = merge(head, nextToMiddle, choice);

}

void sortingFunction(int sortingAlgo, Node** properties,int sortingColumn) {
    auto start = chrono::high_resolution_clock::now();

    sortingAlgo == 1 ? mergeSort(properties, sortingColumn) : quickSort(properties, sortingColumn);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Time taken for sorting: " << duration.count() << " milliseconds" << endl;
}

int main() {
    Node* properties = readPropertiesFromCSV("mudah-apartment-kl-selangor mmz.csv");
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

    return 0;
}
