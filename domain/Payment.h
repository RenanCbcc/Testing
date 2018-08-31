//
// Created by renan on 31/08/18.
//

#ifndef TESTING_PAYMENT_H
#define TESTING_PAYMENT_H


#include <ctime>

class Payment {
public:
    Payment(double value, time_t date);

    double getValue() const;

    time_t getDate() const;

private:
    double value;
    time_t date;

};


#endif //TESTING_PAYMENT_H
