#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

struct Car {
    int id;
    string model;
    double pricePerDay;
    bool isRented = false;
};

const int MAX_CARS = 50;
Car cars[MAX_CARS];
int numCars = 0;

void displayCars();
void rentCar();
void returnCar();
void addCar();
void loadCars();
void saveCars();
void searchById();
void searchByModel();

int main() {
    loadCars();

    int choice;
    do {
        cout << "\nCAR RENTAL SYSTEM\n";
        cout << "1. View Cars\n";
        cout << "2. Rent a Car\n";
        cout << "3. Return a Car\n";
        cout << "4. Add a New Car\n";
        cout << "5. Search Car by ID\n";
        cout << "6. Search Car by Model\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: displayCars(); break;
            case 2: rentCar(); break;
            case 3: returnCar(); break;
            case 4: addCar(); break;
            case 5: searchById(); break;
            case 6: searchByModel(); break;
            case 7: saveCars(); cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 7);

    return 0;
}

void displayCars() {
    cout << "\nAvailable Cars:\n";
    for (int i = 0; i < numCars; i++) {
        if (!cars[i].isRented)
            cout << "ID: " << cars[i].id << ", Model: " << cars[i].model
                 << ", Price per Day: $" << cars[i].pricePerDay << endl;
    }
}

void rentCar() {
    int carId;
    cout << "Enter Car ID to rent: ";
    cin >> carId;

    for (int i = 0; i < numCars; i++) {
        if (cars[i].id == carId && !cars[i].isRented) {
            cars[i].isRented = true;
            cout << "You rented " << cars[i].model << " for $" << cars[i].pricePerDay << " per day.\n";
            saveCars();
            return;
        }
    }
    cout << "Car not available or already rented.\n";
}

void returnCar() {
    int carId;
    cout << "Enter Car ID to return: ";
    cin >> carId;

    for (int i = 0; i < numCars; i++) {
        if (cars[i].id == carId && cars[i].isRented) {
            cars[i].isRented = false;
            cout << "You returned " << cars[i].model << ". Thank you!\n";
            saveCars();
            return;
        }
    }
    cout << "Car not rented.\n";
}

void addCar() {
    if (numCars >= MAX_CARS) {
        cout << "Cannot add more cars. Maximum capacity reached.\n";
        return;
    }

    int newId;
    cout << "Enter car ID: ";
    cin >> newId;

    for (int i = 0; i < numCars; i++) {
        if (cars[i].id == newId) {
            cout << "ID already exists. Please enter a unique ID.\n";
            return;
        }
    }

    cout << "Enter car model: ";
    cin.ignore();
    getline(cin, cars[numCars].model);
    cout << "Enter price per day: ";
    cin >> cars[numCars].pricePerDay;

    cars[numCars].id = newId;
    cars[numCars].isRented = false;
    numCars++;

    ofstream file("car_rent.txt", ios::app);
    if (file.fail()) {
        cout << "Error opening file for appending!\n";
        return;
    }

    file << cars[numCars - 1].id << " " << cars[numCars - 1].model << " "
         << cars[numCars - 1].pricePerDay << " " << cars[numCars - 1].isRented << "\n";
    file.close();

    cout << "New car added.\n";
}

void loadCars() {
    ifstream file("car_rent.txt");
    if (file.fail()) {
        cout << "Error opening file for loading cars.\n";
        return;
    }

    while (file >> cars[numCars].id >> cars[numCars].model >> cars[numCars].pricePerDay >> cars[numCars].isRented) {
        numCars++;
        if (numCars >= MAX_CARS) break;
    }
    file.close();
}

void saveCars() {
    ofstream file("car_rent.txt");
    if (file.fail()) {
        cout << "Error opening file for saving data.\n";
        return;
    }

    for (int i = 0; i < numCars; i++) {
        file << cars[i].id << " " << cars[i].model << " "
             << cars[i].pricePerDay << " " << cars[i].isRented << "\n";
    }

    file.close();
    cout << "Cars saved successfully!\n";
}

void searchById() {
    int searchId;
    bool found = false;

    cout << "Enter Car ID to search: ";
    cin >> searchId;

    for (int i = 0; i < numCars; i++) {
        if (cars[i].id == searchId) {
            cout << "\nCar Found:\n";
            cout << "ID: " << cars[i].id << endl;
            cout << "Model: " << cars[i].model << endl;
            cout << "Price per Day: $" << cars[i].pricePerDay << endl;
            cout << "Status: " << (cars[i].isRented ? "Rented" : "Available") << endl;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Car with ID " << searchId << " not found.\n";
    }
}

void searchByModel() {
    string searchModel;
    bool found = false;

    cout << "Enter Car Model to search: ";
    cin.ignore();
    getline(cin, searchModel);

    cout << "\nSearch Results:\n";
    for (int i = 0; i < numCars; i++) {
        string modelLower = cars[i].model;
        string searchLower = searchModel;
        transform(modelLower.begin(), modelLower.end(), modelLower.begin(), ::tolower);
        transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);

        if (modelLower.find(searchLower) != string::npos) {
            cout << "ID: " << cars[i].id << ", Model: " << cars[i].model
                 << ", Price: $" << cars[i].pricePerDay
                 << ", Status: " << (cars[i].isRented ? "Rented" : "Available") << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No cars found matching '" << searchModel << "'.\n";
    }
}
