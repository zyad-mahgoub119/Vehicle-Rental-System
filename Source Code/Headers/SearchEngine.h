#pragma once
#include <vector>
#include <string>
#include <functional>

#include "Car.h"
#include "Booking.h"
#include "Customer.h"
#include "AVLTree.h"
#include "MergeSort.h"
#include "SortComparators.h"
#include "BinarySearch.h"

// ============================================================
//  SearchEngine
//
//  All searches that operate on vectors are true Binary Search
//  (O(log n)).  The vector is sorted via Merge Sort first when
//  needed.  AVL-tree lookups by ID are O(log n) by structure.
// ============================================================

class SearchEngine {
public:

    // ----------------------------------------------------------
    // Search car by ID — delegates to AVL tree  O(log n)
    // ----------------------------------------------------------
    static Car* searchCarById(AVLTree& tree, int id) {
        return tree.search(id);
    }

    // ----------------------------------------------------------
    // Search booking by ID — Binary Search  O(log n)
    // Vector is sorted by bookingId before searching.
    // ----------------------------------------------------------
    static Booking* searchBookingById(std::vector<Booking>& bookings, int bookingId) {
        if (bookings.empty()) return nullptr;

        // Sort by bookingId so binary search is valid
        std::function<bool(const Booking&, const Booking&)> cmp = cmpBookingById;
        mergeSort(bookings, cmp);

        std::function<int(const Booking&)> key = [](const Booking& b) { return b.bookingId; };
        int idx = binarySearchIndex(bookings, bookingId, key);
        return (idx == -1) ? nullptr : &bookings[idx];
    }

    // ----------------------------------------------------------
    // Search customer by ID — Binary Search  O(log n)
    // ----------------------------------------------------------
    static Customer* searchCustomerById(std::vector<Customer>& customers, int customerId) {
        if (customers.empty()) return nullptr;

        std::function<bool(const Customer&, const Customer&)> cmp = cmpCustomerById;
        mergeSort(customers, cmp);

        std::function<int(const Customer&)> key = [](const Customer& c) { return c.id; };
        int idx = binarySearchIndex(customers, customerId, key);
        return (idx == -1) ? nullptr : &customers[idx];
    }

    // ----------------------------------------------------------
    // Search cars by category — Binary Search (all matches) O(log n)
    // Cars are sorted by category, then all matches are gathered.
    // ----------------------------------------------------------
    static std::vector<Car> searchCarsByCategory(AVLTree& tree,
                                                  const std::string& category)
    {
        std::vector<Car> cars = tree.inOrder();   // already sorted by id
        std::function<bool(const Car&, const Car&)> cmp = cmpCarByCategory;
        mergeSort(cars, cmp);

        std::function<std::string(const Car&)> key =
            [](const Car& c) { return c.category; };
        return binarySearchAll(cars, category, key);
    }

    // ----------------------------------------------------------
    // Search cars by price range — Binary Range Search  O(log n + k)
    // Cars are sorted by price, then the range is extracted.
    // ----------------------------------------------------------
    static std::vector<Car> searchCarsByPriceRange(AVLTree& tree,
                                                    float minPrice,
                                                    float maxPrice)
    {
        std::vector<Car> cars = tree.inOrder();
        std::function<bool(const Car&, const Car&)> cmp = cmpCarByPrice;
        mergeSort(cars, cmp);

        std::function<float(const Car&)> key =
            [](const Car& c) { return c.pricePerDay; };
        return binarySearchRange(cars, minPrice, maxPrice, key);
    }

    // ----------------------------------------------------------
    // Filter available cars  O(n)  — must inspect every car
    // No binary search shortcut possible (availability is boolean,
    // not an ordered key we can sort on meaningfully for this filter)
    // ----------------------------------------------------------
    static std::vector<Car> filterAvailableCars(AVLTree& tree) {
        std::vector<Car> result;
        for (const Car& car : tree.inOrder()) {
            if (car.available) result.push_back(car);
        }
        return result;
    }

    // ----------------------------------------------------------
    // Sort cars by price (ascending) using Merge Sort  O(n log n)
    // ----------------------------------------------------------
    static std::vector<Car> sortCarsByPrice(AVLTree& tree) {
        std::vector<Car> cars = tree.inOrder();
        std::function<bool(const Car&, const Car&)> cmp = cmpCarByPrice;
        mergeSort(cars, cmp);
        return cars;
    }

    // ----------------------------------------------------------
    // Sort cars by year (newest first) using Merge Sort  O(n log n)
    // ----------------------------------------------------------
    static std::vector<Car> sortCarsByYear(AVLTree& tree) {
        std::vector<Car> cars = tree.inOrder();
        std::function<bool(const Car&, const Car&)> cmp = cmpCarByYear;
        mergeSort(cars, cmp);
        return cars;
    }

    // ----------------------------------------------------------
    // Sort bookings by date using Merge Sort  O(n log n)
    // ----------------------------------------------------------
    static std::vector<Booking> sortBookingsByDate(std::vector<Booking> bookings) {
        std::function<bool(const Booking&, const Booking&)> cmp = cmpBookingByDate;
        mergeSort(bookings, cmp);
        return bookings;
    }

    // ----------------------------------------------------------
    // Sort bookings by total price using Merge Sort  O(n log n)
    // ----------------------------------------------------------
    static std::vector<Booking> sortBookingsByPrice(std::vector<Booking> bookings) {
        std::function<bool(const Booking&, const Booking&)> cmp = cmpBookingByPrice;
        mergeSort(bookings, cmp);
        return bookings;
    }
};
