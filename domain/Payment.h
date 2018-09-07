//
// Created by renan on 31/08/18.
//

#ifndef TESTING_PAYMENT_H
#define TESTING_PAYMENT_H

#include <boost/date_time/gregorian/greg_date.hpp>

class Payment {
public:
    Payment(double value, boost::gregorian::date date);

    double getValue() const;

    boost::gregorian::date getDate() const;

private:
    double value;
    boost::gregorian::date date;

};


#endif //TESTING_PAYMENT_H
