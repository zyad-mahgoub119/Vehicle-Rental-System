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

// ============================================================
//  FileManager  —  all static, no instantiation needed.
//  Data folder path is set once via setDataPath().
// ============================================================

class FileManager {
private:
    static string dataPath;

    static void stripCR(string& s) {
        if (!s.empty() && s.back() == '\r') s.pop_back();
    }

    static string path(const string& filename) {
        return dataPath + filename;
    }

    static void ensureExists(const string& fullPath) {
        ifstream f(fullPath);
        if (!f.is_open()) { ofstream create(fullPath); }
    }

public:
    static void setDataPath(const string& p) { dataPath = p; }

    // =========================================================
    // LOAD FUNCTIONS
    // =========================================================

    static void loadCars(AVLTree& tree) {
        string filename = path("cars.txt");
        ensureExists(filename);
        ifstream file(filename);
        if (!file.is_open()) { cerr << "Cannot open " << filename << "\n"; return; }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, brand, model, yearStr, priceStr, category, availStr;
            if (getline(ss, idStr,    '|') && getline(ss, brand,    '|') &&
                getline(ss, model,    '|') && getline(ss, yearStr,  '|') &&
                getline(ss, priceStr, '|') && getline(ss, category, '|') &&
                getline(ss, availStr))
            {
                try {
                    Car car;
                    stripCR(idStr); stripCR(brand); stripCR(model);
                    stripCR(yearStr); stripCR(priceStr); stripCR(category); stripCR(availStr);
                    car.id          = stoi(idStr);
                    car.brand       = brand;
                    car.model       = model;
                    car.year        = stoi(yearStr);
                    car.pricePerDay = stof(priceStr);
                    car.category    = category;
                    car.available   = (stoi(availStr) != 0);
                    tree.insert(car);
                } catch (const exception& e) {
                    cerr << "Parse error (car): " << line << " -> " << e.what() << "\n";
                }
            }
        }
        file.close();
    }

    static void loadCustomers(vector<Customer>& customers) {
        string filename = path("customers.txt");
        ensureExists(filename);
        ifstream file(filename);
        if (!file.is_open()) { cerr << "Cannot open " << filename << "\n"; return; }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, name, phone, email;
            if (getline(ss, idStr, '|') && getline(ss, name, '|') &&
                getline(ss, phone, '|') && getline(ss, email))
            {
                try {
                    Customer c;
                    stripCR(idStr); stripCR(name); stripCR(phone); stripCR(email);
                    c.id    = stoi(idStr);
                    c.name  = name;
                    c.phone = phone;
                    c.email = email;
                    customers.push_back(c);
                } catch (const exception& e) {
                    cerr << "Parse error (customer): " << line << " -> " << e.what() << "\n";
                }
            }
        }
        file.close();
    }

    static void loadBookings(vector<Booking>& bookings) {
        string filename = path("bookings.txt");
        ensureExists(filename);
        ifstream file(filename);
        if (!file.is_open()) { cerr << "Cannot open " << filename << "\n"; return; }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string bIdStr, cIdStr, carIdStr, start, end, priceStr, status;
            if (getline(ss, bIdStr,   '|') && getline(ss, cIdStr,  '|') &&
                getline(ss, carIdStr, '|') && getline(ss, start,   '|') &&
                getline(ss, end,      '|') && getline(ss, priceStr,'|') &&
                getline(ss, status))
            {
                try {
                    Booking b;
                    stripCR(bIdStr); stripCR(cIdStr); stripCR(carIdStr);
                    stripCR(start); stripCR(end); stripCR(priceStr); stripCR(status);
                    b.bookingId  = stoi(bIdStr);
                    b.customerId = stoi(cIdStr);
                    b.carId      = stoi(carIdStr);
                    b.startDate  = start;
                    b.endDate    = end;
                    b.totalPrice = stof(priceStr);
                    b.status     = status;
                    bookings.push_back(b);
                } catch (const exception& e) {
                    cerr << "Parse error (booking): " << line << " -> " << e.what() << "\n";
                }
            }
        }
        file.close();
    }

    // =========================================================
    // SAVE FUNCTIONS
    // =========================================================

    static void saveCars(AVLTree& tree) {
        ofstream f(path("cars.txt"), ios::trunc);
        if (!f) { cerr << "Cannot write cars.txt\n"; return; }
        for (const Car& car : tree.inOrder())
            f << car.id << "|" << car.brand << "|" << car.model << "|"
              << car.year << "|" << car.pricePerDay << "|"
              << car.category << "|" << (car.available ? 1 : 0) << "\n";
    }

    static void saveCustomers(const vector<Customer>& customers) {
        ofstream f(path("customers.txt"), ios::trunc);
        if (!f) { cerr << "Cannot write customers.txt\n"; return; }
        for (const Customer& c : customers)
            f << c.id << "|" << c.name << "|" << c.phone << "|" << c.email << "\n";
    }

    static void saveBookings(const vector<Booking>& bookings) {
        ofstream f(path("bookings.txt"), ios::trunc);
        if (!f) { cerr << "Cannot write bookings.txt\n"; return; }
        for (const Booking& b : bookings)
            f << b.bookingId << "|" << b.customerId << "|" << b.carId << "|"
              << b.startDate << "|" << b.endDate << "|"
              << b.totalPrice << "|" << b.status << "\n";
    }

    static void saveAll(AVLTree& tree,
                        const vector<Customer>& customers,
                        const vector<Booking>& bookings)
    {
        saveCars(tree);
        saveCustomers(customers);
        saveBookings(bookings);
    }
};

// Definition of static member
inline string FileManager::dataPath = "data/";
