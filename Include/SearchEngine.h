#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "Car.h"
#include "Booking.h"
#include "AVLTree.h"
using namespace std;
class SearchEngine {
public:

    // -------------------------------------------------------------------------
    // Search by ID — just delegates to the AVL tree (already O(log n))
    // -------------------------------------------------------------------------
    static Car* searchById(AVLTree& tree, int id) {
        return tree.search(id);
    }

    // -------------------------------------------------------------------------
    // Search by price range — gets sorted list, then filters
    // -------------------------------------------------------------------------
    static vector<Car> searchByPriceRange(AVLTree& tree, float minPrice, float maxPrice) {
        vector<Car> result;
        for (const Car& car : tree.inOrder()) {
            if (car.pricePerDay >= minPrice && car.pricePerDay <= maxPrice) {
                result.push_back(car);
            }
        }
        return result;
    }

    // -------------------------------------------------------------------------
    // Filter by category
    // -------------------------------------------------------------------------
    static vector<Car> filterByCategory(AVLTree& tree, const :string& category) {
        vector<Car> result;
        for (const Car& car : tree.inOrder()) {
            if (car.category == category) {
                result.push_back(car);
            }
        }
        return result;
    }

    // -------------------------------------------------------------------------
    // Filter available cars only
    // -------------------------------------------------------------------------
    static vector <Car> filterByAvailability(AVLTree& tree) {
        vector<Car> result;
        for (const Car& car : tree.inOrder()) {
            if (car.available) {
                result.push_back(car);
            }
        }
        return result;
    } 

    // -------------------------------------------------------------------------
    // Booking lookup by ID — linear scan on the vector (bookings are few)
    // -------------------------------------------------------------------------
    static Booking* lookupBookingById(vector<Booking>& bookings, int bookingId) {
        for (Booking& b : bookings) {
            if (b.bookingId == bookingId) {
                return &b;
            }
        }
        return nullptr;
    }
};
