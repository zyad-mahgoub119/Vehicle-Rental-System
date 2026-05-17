#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Car.h"
#include "Customer.h"
#include "Booking.h"
#include "AVLTree.h"

using namespace std;

class FileManager {
private:
    static void checkAndCreateFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            ofstream createFile(filename);
            createFile.close();
        }
        else {
            file.close();
        }
    }

public:
    // =========================================================================
    // LOADING FUNCTIONS (Read from file -> Populate Data Structures)
    // =========================================================================

    // Reads cars.txt line by line, splits by '|', inserts each Car into the AVL tree

    static void loadCars(AVLTree& tree) {
        string filename = "cars.txt";
        checkAndCreateFile(filename);

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << " for reading!\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue; // Skip empty lines

            stringstream ss(line);
            string idStr, brand, model, yearStr, priceStr, category, availStr;

            // Extract each token separated by '|'
            if (getline(ss, idStr, '|') &&
                getline(ss, brand, '|') &&
                getline(ss, model, '|') &&
                getline(ss, yearStr, '|') &&
                getline(ss, priceStr, '|') &&
                getline(ss, category, '|') &&
                getline(ss, availStr)) {

                try {
                    Car car;
                    car.id = stoi(idStr);
                    car.brand = brand;
                    car.model = model;
                    car.year = stoi(yearStr);
                    car.pricePerDay = stof(priceStr);
                    car.category = category;
                    car.available = (stoi(availStr) != 0); // Converts '1' to true, '0' to false

                    tree.insert(car);
                }
                catch (const exception& e) {
                    cerr << "Error parsing car line: " << line << " -> " << e.what() << endl;
                }
            }
        }
        file.close();
    }

    // Reads customers.txt, splits by '|', and fills the customer vector
    static void loadCustomers(vector<Customer>& customers) {
        string filename = "customers.txt";
        checkAndCreateFile(filename);

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << " for reading!\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, name, phone, email;

            if (getline(ss, idStr, '|') &&
                getline(ss, name, '|') &&
                getline(ss, phone, '|') &&
                getline(ss, email)) {

                try {
                    Customer cust;
                    cust.id = stoi(idStr);
                    cust.name = name;
                    cust.phone = phone;
                    cust.email = email;

                    customers.push_back(cust);
                }
                catch (const exception& e) {
                    cerr << "Error parsing customer line: " << line << " -> " << e.what() << endl;
                }
            }
        }
        file.close();
    }

    // Reads bookings.txt, splits by '|', and fills the booking vector

    static void loadBookings(vector<Booking>& bookings) {
        string filename = "bookings.txt";
        checkAndCreateFile(filename);

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening " << filename << " for reading!\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string bIdStr, cIdStr, carIdStr, start, end, priceStr, status;

            if (getline(ss, bIdStr, '|') &&
                getline(ss, cIdStr, '|') &&
                getline(ss, carIdStr, '|') &&
                getline(ss, start, '|') &&
                getline(ss, end, '|') &&
                getline(ss, priceStr, '|') &&
                getline(ss, status)) {

                try {
                    Booking b;
                    b.bookingId = stoi(bIdStr);
                    b.customerId = stoi(cIdStr);
                    b.carId = stoi(carIdStr);
                    b.startDate = start;
                    b.endDate = end;
                    b.totalPrice = stof(priceStr);
                    b.status = status;

                    bookings.push_back(b);
                }
                catch (const exception& e) {
                    cerr << "Error parsing booking line: " << line << " -> " << e.what() << endl;
                }
            }
        }
        file.close();
    }

    // =========================================================================
    // SAVING FUNCTIONS (Memory Structures -> Write back to text files)
    // =========================================================================


    static void saveAll(AVLTree& tree, const vector<Customer>& customers, const vector<Booking>& bookings) {

        // 1. Save Cars from AVLTree

        ofstream carFile("cars.txt", ios::out | ios::trunc);
        if (carFile.is_open()) {
            vector<Car> sortedCars = tree.inOrder();
            for (const auto& car : sortedCars) {
                carFile << car.id << "|"
                    << car.brand << "|"
                    << car.model << "|"
                    << car.year << "|"
                    << car.pricePerDay << "|"
                    << car.category << "|"
                    << (car.available ? 1 : 0) << "\n";
            }
            carFile.close();
        }
        else {
            cerr << "Error: Could not open cars.txt for writing.\n";
        }

        // 2. Save Customers from Vector
        ofstream custFile("customers.txt", ios::out | ios::trunc);
        if (custFile.is_open()) {
            for (const auto& cust : customers) {
                custFile << cust.id << "|"
                    << cust.name << "|"
                    << cust.phone << "|"
                    << cust.email << "\n";
            }
            custFile.close();
        }
        else {
            cerr << "Error: Could not open customers.txt for writing.\n";
        }

        // 3. Save Bookings from Vector
        ofstream bookFile("bookings.txt", ios::out | ios::trunc);
        if (bookFile.is_open()) {
            for (const auto& b : bookings) {
                bookFile << b.bookingId << "|"
                    << b.customerId << "|"
                    << b.carId << "|"
                    << b.startDate << "|"
                    << b.endDate << "|"
                    << b.totalPrice << "|"
                    << b.status << "\n";
            }
            bookFile.close();
        }
        else {
            cerr << "Error: Could not open bookings.txt for writing.\n";
        }
    }
};
