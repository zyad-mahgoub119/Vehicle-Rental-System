

#include "Car.h"
#include "Booking.h"



// car comparator


bool sortByPriceperday(const Car& a, const Car& b)
{
    return a.pricePerDay < b.pricePerDay;
}


bool sortByYear(const Car& a, const Car& b)
{
    // newest first

    return a.year > b.year;
}

bool sortByCategory(const Car& a, const Car& b)
{
    return a.category < b.category;
}



// Booking Comparators


bool sortBookingsByDate(const Booking& a, const Booking& b)
{
    return a.startDate < b.startDate;
}

bool sortBookingsByTotalPrice(const Booking& a, const Booking& b)
{
    return a.totalPrice < b.totalPrice;
}