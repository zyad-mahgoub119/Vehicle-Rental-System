#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

#include "Customer.h"
#include "Booking.h"
#include "FileManager.h"

using namespace std;

class CustomerManager
{
private:
    vector<Customer> customers;
    FileManager fileManager;

    bool isValidName(string name);
    bool isValidPhone(string phone);
    bool isValidEmail(string email);

public:
    CustomerManager();

    void addCustomer(string name, string phone, string email);
    void removeCustomer(int id);
    void updateCustomer(int id, string name, string phone, string email);

    void displayAll();

    Customer *findById(int id);
    vector<Customer> findByName(string name);

    void printProfile(int customerId, vector<Booking> &bookings);
};

// ================= constructor =================
CustomerManager::CustomerManager()
{
    customers = fileManager.loadCustomers();
}

// ================= add =================
void CustomerManager::addCustomer(string name, string phone, string email)
{
    if (!isValidName(name) || !isValidPhone(phone) || !isValidEmail(email))
    {
        cout << "Invalid data\n";
        return;
    }

    int maxId = 0;

    for (int i = 0; i < customers.size(); i++)
    {
        if (customers[i].id > maxId)
            maxId = customers[i].id;
    }

    Customer c;
    c.id = maxId + 1;
    c.name = name;
    c.phone = phone;
    c.email = email;

    customers.push_back(c);

    cout << "added\n";
    fileManager.saveCustomers(customers);
}

// ================= remove =================
void CustomerManager::removeCustomer(int id)
{
    for (int i = 0; i < customers.size(); i++)
    {
        if (customers[i].id == id)
        {
            customers.erase(customers.begin() + i);
            cout << "removed\n";
            fileManager.saveCustomers(customers);
            return;
        }
    }

    cout << "not found\n";
}

// ================= update =================
void CustomerManager::updateCustomer(int id, string name, string phone, string email)
{
    Customer *c = findById(id);

    if (c == nullptr)
    {
        cout << "not found\n";
        return;
    }

    if (name != "" && isValidName(name))
        c->name = name;

    if (phone != "" && isValidPhone(phone))
        c->phone = phone;

    if (email != "" && isValidEmail(email))
        c->email = email;

    cout << "updated\n";
    fileManager.saveCustomers(customers);
}

// ================= display =================
void CustomerManager::displayAll()
{
    cout << "ID  Name  Phone  Email\n";

    for (int i = 0; i < customers.size(); i++)
    {
        cout << customers[i].id << " "
             << customers[i].name << " "
             << customers[i].phone << " "
             << customers[i].email << "\n";
    }
}

// ================= find by id =================
Customer *CustomerManager::findById(int id)
{
    for (int i = 0; i < customers.size(); i++)
    {
        if (customers[i].id == id)
            return &customers[i];
    }

    return nullptr;
}

// ================= find by name =================
vector<Customer> CustomerManager::findByName(string name)
{
    vector<Customer> result;

    for (int i = 0; i < customers.size(); i++)
    {
        string cname = customers[i].name;

        for (int j = 0; j < cname.size(); j++)
            cname[j] = tolower(cname[j]);

        string key = name;
        for (int j = 0; j < key.size(); j++)
            key[j] = tolower(key[j]);

        if (cname.find(key) != string::npos)
            result.push_back(customers[i]);
    }

    return result;
}

// ================= profile =================
void CustomerManager::printProfile(int customerId, vector<Booking> &bookings)
{
    Customer *c = findById(customerId);

    if (!c)
    {
        cout << "not found\n";
        return;
    }

    cout << "ID: " << c->id << "\n";
    cout << "Name: " << c->name << "\n";
    cout << "Phone: " << c->phone << "\n";
    cout << "Email: " << c->email << "\n";

    cout << "\nBookings:\n";

    bool found = false;

    for (int i = 0; i < bookings.size(); i++)
    {
        if (bookings[i].customerId == customerId)
        {
            found = true;
            cout << bookings[i].bookingId << " "
                 << bookings[i].carId << " "
                 << bookings[i].status << "\n";
        }
    }

    if (!found)
        cout << "no bookings\n";
}

// ================= validation =================
bool CustomerManager::isValidName(string name)
{
    if (name == "")
        return false;

    for (int i = 0; i < name.size(); i++)
        if (isdigit(name[i]))
            return false;

    return true;
}

bool CustomerManager::isValidPhone(string phone)
{
    if (phone.size() < 10 || phone.size() > 11)
        return false;

    for (int i = 0; i < phone.size(); i++)
        if (!isdigit(phone[i]))
            return false;

    return true;
}

bool CustomerManager::isValidEmail(string email)
{
    if (email.find('@') == string::npos)
        return false;
    if (email.find('.') == string::npos)
        return false;

    return true;
}