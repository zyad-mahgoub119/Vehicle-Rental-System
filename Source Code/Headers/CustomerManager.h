#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <cctype>

#include "Customer.h"
#include "Booking.h"
#include "FileManager.h"
#include "MergeSort.h"
#include "SortComparators.h"
#include "BinarySearch.h"

using namespace std;

// ============================================================
//  CustomerManager
//  Wraps the customers vector and provides CRUD + search.
//  Uses Binary Search (O(log n)) for lookups by ID.
// ============================================================

class CustomerManager {
private:
    vector<Customer> customers;

    // ---- Validation helpers ----
    bool isValidName(const string& name) {
        if (name.empty()) return false;
        for (char c : name)
            if (isdigit(c)) return false;
        return true;
    }

    bool isValidPhone(const string& phone) {
        if (phone.size() < 10 || phone.size() > 15) return false;
        for (char c : phone)
            if (!isdigit(c) && c != '+' && c != '-') return false;
        return true;
    }

    bool isValidEmail(const string& email) {
        return email.find('@') != string::npos &&
               email.find('.') != string::npos;
    }

    // ---- Keep vector sorted by ID so binary search always works ----
    void sortById() {
        function<bool(const Customer&, const Customer&)> cmp = cmpCustomerById;
        mergeSort(customers, cmp);
    }

public:
    CustomerManager() {
        FileManager::loadCustomers(customers);
        sortById();
    }

    // ---- Add ----
    void addCustomer(const string& name, const string& phone, const string& email) {
        if (!isValidName(name) || !isValidPhone(phone) || !isValidEmail(email)) {
            cout << "Invalid customer data.\n";
            return;
        }
        int maxId = 0;
        for (const Customer& c : customers)
            if (c.id > maxId) maxId = c.id;

        Customer c;
        c.id    = maxId + 1;
        c.name  = name;
        c.phone = phone;
        c.email = email;
        customers.push_back(c);
        sortById();   // maintain sort invariant

        cout << "Customer added (ID " << c.id << ").\n";
        FileManager::saveCustomers(customers);
    }

    // ---- Remove ----
    void removeCustomer(int id) {
        for (int i = 0; i < (int)customers.size(); i++) {
            if (customers[i].id == id) {
                customers.erase(customers.begin() + i);
                cout << "Customer " << id << " removed.\n";
                FileManager::saveCustomers(customers);
                return;
            }
        }
        cout << "Customer not found.\n";
    }

    // ---- Update ----
    void updateCustomer(int id, const string& name,
                        const string& phone, const string& email)
    {
        Customer* c = findById(id);
        if (!c) { cout << "Customer not found.\n"; return; }

        if (!name.empty()  && isValidName(name))   c->name  = name;
        if (!phone.empty() && isValidPhone(phone))  c->phone = phone;
        if (!email.empty() && isValidEmail(email))  c->email = email;

        cout << "Customer " << id << " updated.\n";
        FileManager::saveCustomers(customers);
    }

    // ---- Display all ----
    void displayAll() const {
        cout << "ID    Name                 Phone          Email\n";
        cout << string(65, '-') << "\n";
        for (const Customer& c : customers)
            cout << c.id << "\t" << c.name << "\t" << c.phone << "\t" << c.email << "\n";
    }

    // ---- Find by ID — Binary Search O(log n) ----
    Customer* findById(int id) {
        // vector is always sorted by ID
        function<int(const Customer&)> key = [](const Customer& c) { return c.id; };
        int idx = binarySearchIndex(customers, id, key);
        return (idx == -1) ? nullptr : &customers[idx];
    }

    // ---- Find by name — linear scan (name not indexed) ----
    vector<Customer> findByName(const string& name) const {
        vector<Customer> result;
        string lowerKey = name;
        for (char& c : lowerKey) c = tolower(c);

        for (const Customer& cust : customers) {
            string lowerName = cust.name;
            for (char& c : lowerName) c = tolower(c);
            if (lowerName.find(lowerKey) != string::npos)
                result.push_back(cust);
        }
        return result;
    }

    // ---- Print profile with booking history ----
    void printProfile(int customerId, const vector<Booking>& bookings) const {
        const Customer* c = nullptr;
        for (const Customer& cu : customers)
            if (cu.id == customerId) { c = &cu; break; }

        if (!c) { cout << "Customer not found.\n"; return; }

        cout << "\n=== Customer Profile ===\n";
        cout << "ID:    " << c->id    << "\n";
        cout << "Name:  " << c->name  << "\n";
        cout << "Phone: " << c->phone << "\n";
        cout << "Email: " << c->email << "\n";
        cout << "\nBooking History:\n";

        bool found = false;
        for (const Booking& b : bookings) {
            if (b.customerId == customerId) {
                cout << "  Booking #" << b.bookingId
                     << " | Car ID: " << b.carId
                     << " | " << b.startDate << " -> " << b.endDate
                     << " | $" << b.totalPrice
                     << " | " << b.status << "\n";
                found = true;
            }
        }
        if (!found) cout << "  No bookings.\n";
    }

    const vector<Customer>& getAll() const { return customers; }
};
