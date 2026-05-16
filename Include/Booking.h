#pragma once
#include <string>

struct Booking {
    int bookingId;
    int customerId;
    int carId;
    std::string startDate;
    std::string endDate;
    float totalPrice;
    std::string status;
};
