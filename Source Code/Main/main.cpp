// ============================================================
//  Vehicle Rental System — Main Entry Point
//  Algorithms: Merge Sort (all sorting) | Binary Search (all lookups)
//  Data Structure: AVL Tree (car storage/retrieval by ID)
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <functional>
#include <limits>

#include "Car.h"
#include "Customer.h"
#include "Booking.h"
#include "AVLTree.h"
#include "FileManager.h"
#include "CustomerManager.h"
#include "SearchEngine.h"
#include "MergeSort.h"
#include "SortComparators.h"
#include "BinarySearch.h"

using namespace std;

// ---- Global state ----
AVLTree        carTree;
vector<Booking> bookings;
CustomerManager custManager;   // loads customers internally

// ---- Helpers ----
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printCar(const Car& c) {
    cout << fixed << setprecision(2);
    cout << "  [" << c.id << "] " << c.brand << " " << c.model
         << "  Year: " << c.year
         << "  $" << c.pricePerDay << "/day"
         << "  Category: " << c.category
         << "  " << (c.available ? "(Available)" : "(Rented)") << "\n";
}

void printBooking(const Booking& b) {
    cout << fixed << setprecision(2);
    cout << "  Booking #" << b.bookingId
         << " | Customer: " << b.customerId
         << " | Car: " << b.carId
         << " | " << b.startDate << " -> " << b.endDate
         << " | $" << b.totalPrice
         << " | " << b.status << "\n";
}

int nextBookingId() {
    int maxId = 9000;
    for (const Booking& b : bookings)
        if (b.bookingId > maxId) maxId = b.bookingId;
    return maxId + 1;
}

// Compute days between "YYYY-MM-DD" strings (simple arithmetic)
int daysBetween(const string& start, const string& end) {
    auto toInt = [](const string& s) {
        int y = stoi(s.substr(0,4));
        int m = stoi(s.substr(5,2));
        int d = stoi(s.substr(8,2));
        return y*365 + m*30 + d;   // rough but sufficient
    };
    int diff = toInt(end) - toInt(start);
    return diff > 0 ? diff : 1;
}

// ================================================================
//  MENUS
// ================================================================

// ---- Car Management ----
void menuCars() {
    while (true) {
        cout << "\n=== Car Management ===\n"
             << "1. View all cars (sorted by ID)\n"
             << "2. Sort cars by price\n"
             << "3. Sort cars by year (newest first)\n"
             << "4. Search car by ID\n"
             << "5. Search cars by category\n"
             << "6. Search cars by price range\n"
             << "7. Show available cars only\n"
             << "8. Add car\n"
             << "9. Remove car\n"
             << "0. Back\n"
             << "Choice: ";
        int ch; cin >> ch; clearInput();

        if (ch == 0) break;

        if (ch == 1) {
            cout << "\n--- All Cars ---\n";
            for (const Car& c : carTree.inOrder()) printCar(c);
        }
        else if (ch == 2) {
            cout << "\n--- Cars by Price (cheapest first) ---\n";
            for (const Car& c : SearchEngine::sortCarsByPrice(carTree)) printCar(c);
        }
        else if (ch == 3) {
            cout << "\n--- Cars by Year (newest first) ---\n";
            for (const Car& c : SearchEngine::sortCarsByYear(carTree)) printCar(c);
        }
        else if (ch == 4) {
            cout << "Enter car ID: "; int id; cin >> id; clearInput();
            Car* c = SearchEngine::searchCarById(carTree, id);
            if (c) printCar(*c); else cout << "Car not found.\n";
        }
        else if (ch == 5) {
            cout << "Enter category (Sedan/Electric/SUV/...): ";
            string cat; getline(cin, cat);
            auto results = SearchEngine::searchCarsByCategory(carTree, cat);
            if (results.empty()) cout << "No cars in that category.\n";
            else for (const Car& c : results) printCar(c);
        }
        else if (ch == 6) {
            float mn, mx;
            cout << "Min price/day: "; cin >> mn;
            cout << "Max price/day: "; cin >> mx; clearInput();
            auto results = SearchEngine::searchCarsByPriceRange(carTree, mn, mx);
            if (results.empty()) cout << "No cars in that price range.\n";
            else for (const Car& c : results) printCar(c);
        }
        else if (ch == 7) {
            cout << "\n--- Available Cars ---\n";
            auto results = SearchEngine::filterAvailableCars(carTree);
            if (results.empty()) cout << "No cars available.\n";
            else for (const Car& c : results) printCar(c);
        }
        else if (ch == 8) {
            Car c;
            cout << "Car ID: ";          cin >> c.id;       clearInput();
            cout << "Brand: ";           getline(cin, c.brand);
            cout << "Model: ";           getline(cin, c.model);
            cout << "Year: ";            cin >> c.year;     clearInput();
            cout << "Price per day: ";   cin >> c.pricePerDay; clearInput();
            cout << "Category: ";        getline(cin, c.category);
            c.available = true;
            carTree.insert(c);
            FileManager::saveCars(carTree);
            cout << "Car added.\n";
        }
        else if (ch == 9) {
            cout << "Enter car ID to remove: "; int id; cin >> id; clearInput();
            carTree.remove(id);
            FileManager::saveCars(carTree);
            cout << "Car removed (if it existed).\n";
        }
    }
}

// ---- Customer Management ----
void menuCustomers() {
    while (true) {
        cout << "\n=== Customer Management ===\n"
             << "1. View all customers\n"
             << "2. Search by ID\n"
             << "3. Search by name\n"
             << "4. View customer profile (with bookings)\n"
             << "5. Add customer\n"
             << "6. Update customer\n"
             << "7. Remove customer\n"
             << "0. Back\n"
             << "Choice: ";
        int ch; cin >> ch; clearInput();
        if (ch == 0) break;

        if (ch == 1) {
            custManager.displayAll();
        }
        else if (ch == 2) {
            cout << "Customer ID: "; int id; cin >> id; clearInput();
            Customer* c = custManager.findById(id);
            if (c) cout << "  [" << c->id << "] " << c->name << " | " << c->phone << " | " << c->email << "\n";
            else   cout << "Not found.\n";
        }
        else if (ch == 3) {
            cout << "Name (partial ok): "; string name; getline(cin, name);
            auto res = custManager.findByName(name);
            if (res.empty()) cout << "No match.\n";
            else for (const Customer& c : res)
                cout << "  [" << c.id << "] " << c.name << " | " << c.phone << " | " << c.email << "\n";
        }
        else if (ch == 4) {
            cout << "Customer ID: "; int id; cin >> id; clearInput();
            custManager.printProfile(id, bookings);
        }
        else if (ch == 5) {
            string name, phone, email;
            cout << "Name: ";  getline(cin, name);
            cout << "Phone: "; getline(cin, phone);
            cout << "Email: "; getline(cin, email);
            custManager.addCustomer(name, phone, email);
        }
        else if (ch == 6) {
            int id; string name, phone, email;
            cout << "Customer ID to update: "; cin >> id; clearInput();
            cout << "New name  (Enter to skip): "; getline(cin, name);
            cout << "New phone (Enter to skip): "; getline(cin, phone);
            cout << "New email (Enter to skip): "; getline(cin, email);
            custManager.updateCustomer(id, name, phone, email);
        }
        else if (ch == 7) {
            cout << "Customer ID to remove: "; int id; cin >> id; clearInput();
            custManager.removeCustomer(id);
        }
    }
}

// ---- Booking Management ----
void menuBookings() {
    while (true) {
        cout << "\n=== Booking Management ===\n"
             << "1. View all bookings (sorted by date)\n"
             << "2. View all bookings (sorted by price)\n"
             << "3. Search booking by ID\n"
             << "4. Create booking\n"
             << "5. Cancel booking\n"
             << "0. Back\n"
             << "Choice: ";
        int ch; cin >> ch; clearInput();
        if (ch == 0) break;

        if (ch == 1) {
            auto sorted = SearchEngine::sortBookingsByDate(bookings);
            if (sorted.empty()) cout << "No bookings.\n";
            else for (const Booking& b : sorted) printBooking(b);
        }
        else if (ch == 2) {
            auto sorted = SearchEngine::sortBookingsByPrice(bookings);
            if (sorted.empty()) cout << "No bookings.\n";
            else for (const Booking& b : sorted) printBooking(b);
        }
        else if (ch == 3) {
            cout << "Booking ID: "; int id; cin >> id; clearInput();
            Booking* b = SearchEngine::searchBookingById(bookings, id);
            if (b) printBooking(*b); else cout << "Booking not found.\n";
        }
        else if (ch == 4) {
            int custId, carId;
            string start, end;
            cout << "Customer ID: "; cin >> custId; clearInput();
            cout << "Car ID: ";      cin >> carId;  clearInput();
            cout << "Start date (YYYY-MM-DD): "; getline(cin, start);
            cout << "End date   (YYYY-MM-DD): "; getline(cin, end);

            // Validate customer
            Customer* cust = custManager.findById(custId);
            if (!cust) { cout << "Customer not found.\n"; continue; }

            // Validate car
            Car* car = SearchEngine::searchCarById(carTree, carId);
            if (!car)             { cout << "Car not found.\n"; continue; }
            if (!car->available)  { cout << "Car is not available.\n"; continue; }

            // Create booking
            Booking b;
            b.bookingId  = nextBookingId();
            b.customerId = custId;
            b.carId      = carId;
            b.startDate  = start;
            b.endDate    = end;
            b.totalPrice = car->pricePerDay * daysBetween(start, end);
            b.status     = "Active";

            car->available = false;   // mark rented
            bookings.push_back(b);

            FileManager::saveAll(carTree, custManager.getAll(), bookings);
            cout << "Booking created! ID: " << b.bookingId
                 << " | Total: $" << fixed << setprecision(2) << b.totalPrice << "\n";
        }
        else if (ch == 5) {
            cout << "Booking ID to cancel: "; int id; cin >> id; clearInput();
            bool found = false;
            for (Booking& b : bookings) {
                if (b.bookingId == id) {
                    b.status = "Cancelled";
                    // Free the car
                    Car* car = SearchEngine::searchCarById(carTree, b.carId);
                    if (car) car->available = true;
                    found = true;
                    cout << "Booking " << id << " cancelled.\n";
                    break;
                }
            }
            if (!found) cout << "Booking not found.\n";
            FileManager::saveAll(carTree, custManager.getAll(), bookings);
        }
    }
}

// ================================================================
//  MAIN
// ================================================================
int main() {
    // Set data path relative to executable location
    FileManager::setDataPath("data/");

    // Load data
    FileManager::loadCars(carTree);
    FileManager::loadBookings(bookings);
    // CustomerManager loads customers in its constructor

    cout << "========================================\n";
    cout << "      Vehicle Rental System             \n";
    cout << "  Sorting: Merge Sort | Search: Binary  \n";
    cout << "  Car Store: AVL Tree                   \n";
    cout << "========================================\n";

    while (true) {
        cout << "\n=== Main Menu ===\n"
             << "1. Cars\n"
             << "2. Customers\n"
             << "3. Bookings\n"
             << "0. Exit\n"
             << "Choice: ";
        int ch; cin >> ch; clearInput();

        if (ch == 0) {
            FileManager::saveAll(carTree, custManager.getAll(), bookings);
            cout << "Data saved. Goodbye!\n";
            break;
        }
        else if (ch == 1) menuCars();
        else if (ch == 2) menuCustomers();
        else if (ch == 3) menuBookings();
        else cout << "Invalid choice.\n";
    }

    return 0;
}
