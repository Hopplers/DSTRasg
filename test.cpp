#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


struct Property {
    string ads_id;
    string prop_name;
    int completion_year;
    double monthly_rent;
    string location;
    string property_type;
    int rooms;
    int parking;
    int bathroom;
    double size;
    string furnished;

    // Constructor
    Property(string ads_id, string prop_name, int completion_year, double monthly_rent, 
             string location, string property_type, int rooms, int parking, 
             int bathroom, double size, string furnished)
        : ads_id(ads_id), prop_name(prop_name), completion_year(completion_year), monthly_rent(monthly_rent), 
          location(location), property_type(property_type), rooms(rooms), parking(parking), 
          bathroom(bathroom), size(size), furnished(furnished) {}
};


// Read CSV file and return vector of Property
vector<Property> readPropertiesFromCSV(const string& filename) {
    vector<Property> properties;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return properties;
    }

    string line;
    // Removing header
    getline(file, line); 

    while (getline(file, line)) {
        istringstream ss(line);
        string ads_id, prop_name, location, property_type, furnished;
        int completion_year, rooms, parking, bathroom;
        double monthly_rent, size;

        getline(ss, ads_id, ',');
        getline(ss, prop_name, ',');
        ss >> completion_year; ss.ignore();
        ss >> monthly_rent; ss.ignore();
        getline(ss, location, ',');
        getline(ss, property_type, ',');
        ss >> rooms; ss.ignore();
        ss >> parking; ss.ignore();
        ss >> bathroom; ss.ignore();
        ss >> size; ss.ignore();
        getline(ss, furnished, ',');

        properties.emplace_back(ads_id, prop_name, completion_year, monthly_rent, location, property_type, rooms, parking, bathroom, size, furnished);
    }

    file.close();
    return properties;
}

void printProperty(const Property& property) {
    cout << "Ads ID: " << property.ads_id << "\n"
         << "Property Name: " << property.prop_name << "\n"
         << "Completion Year: " << property.completion_year << "\n"
         << "Monthly Rent: " << property.monthly_rent << "\n"
         << "Location: " << property.location << "\n"
         << "Property Type: " << property.property_type << "\n"
         << "Rooms: " << property.rooms << "\n"
         << "Parking Spaces: " << property.parking << "\n"
         << "Bathrooms: " << property.bathroom << "\n"
         << "Size: " << property.size << "\n"
         << "Furnished: " << property.furnished << "\n";
}

int main () {
    vector<Property> propertyList = readPropertiesFromCSV ("output.csv");

    for (const auto& property : propertyList) {
        printProperty(property);
        cout << endl;  
    }

    return 0;
}
