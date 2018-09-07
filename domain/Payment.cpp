//
// Created by renan on 31/08/18.
//

#include "Payment.h"

Payment::Payment(double value, boost::gregorian::date date) : value(value), date(date) {}

double Payment::getValue() const {
    return value;
}

boost::gregorian::date Payment::getDate() const {
    return date;
}


