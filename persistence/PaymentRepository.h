//
// Created by renan on 07/09/18.
//

#ifndef TESTING_PAYMENTREPOSITORY_H
#define TESTING_PAYMENTREPOSITORY_H


#include "../domain/Payment.h"

class PaymentRepository {
public:
    virtual void save (Payment&);
};


#endif //TESTING_PAYMENTREPOSITORY_H
