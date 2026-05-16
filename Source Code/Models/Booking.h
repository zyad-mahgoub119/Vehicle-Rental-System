#pragma once

using namespace std;
#include <string>

struct Booking {
    int bookingId;
    int customerId;
    int carId;
    string startDate;
    string endDate;
    float totalPrice;
    string status;
};
