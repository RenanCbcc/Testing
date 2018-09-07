//
// Created by renan on 07/09/18.
//

#ifndef TESTING_PAYMENTGENERATOR_H
#define TESTING_PAYMENTGENERATOR_H

#include "../domain/Payment.h"
#include "../persistence/AuctionDAO.h"
#include "Evaluator.h"
#include "../persistence/PaymentRepository.h"

class PaymentGenerator {
public:
    PaymentGenerator(AuctionDAO &, Evaluator &, PaymentRepository &);

    boost::gregorian::date getBusinessDay();

    void generate();

private:
    AuctionDAO &dao;
    Evaluator &evaluator;
    PaymentRepository &paymentRepository;
};


#endif //TESTING_PAYMENTGENERATOR_H
