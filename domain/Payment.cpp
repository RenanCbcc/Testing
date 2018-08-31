//
// Created by renan on 31/08/18.
//

#include "Payment.h"

double Payment::getValue() const {
    return value;
}

time_t Payment::getDate() const {
    return date;
}

Payment::Payment(double value, time_t date) : value(value), date(date) {}
