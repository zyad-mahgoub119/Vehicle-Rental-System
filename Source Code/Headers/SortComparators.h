#pragma once
#include "Car.h"
#include "Booking.h"
#include "Customer.h"

// ============================================================
//  Comparators for Merge Sort
//  Each returns true when 'a' should come BEFORE 'b'
// ============================================================

// --- Car comparators ---
inline bool cmpCarByPrice(const Car& a, const Car& b)     { return a.pricePerDay < b.pricePerDay; }
inline bool cmpCarByPriceDesc(const Car& a, const Car& b) { return a.pricePerDay > b.pricePerDay; }
inline bool cmpCarByYear(const Car& a, const Car& b)      { return a.year > b.year; }   // newest first
inline bool cmpCarByCategory(const Car& a, const Car& b)  { return a.category < b.category; }
inline bool cmpCarById(const Car& a, const Car& b)        { return a.id < b.id; }

// --- Booking comparators ---
inline bool cmpBookingByDate(const Booking& a, const Booking& b)  { return a.startDate < b.startDate; }
inline bool cmpBookingByPrice(const Booking& a, const Booking& b) { return a.totalPrice < b.totalPrice; }
inline bool cmpBookingById(const Booking& a, const Booking& b)    { return a.bookingId < b.bookingId; }

// --- Customer comparators ---
inline bool cmpCustomerByName(const Customer& a, const Customer& b) { return a.name < b.name; }
inline bool cmpCustomerById(const Customer& a, const Customer& b)   { return a.id < b.id; }
