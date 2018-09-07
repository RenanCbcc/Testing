//
// Created by renan on 07/09/18.
//

#include "PaymentGenerator.h"


PaymentGenerator::PaymentGenerator(AuctionDAO &dao, Evaluator &ev, PaymentRepository &pr) :
        dao(dao), evaluator(ev), paymentRepository(pr) {}


void PaymentGenerator::generate() {
    for (auto auction: dao.closed()) {
        PaymentGenerator::evaluator.evaluate(auction);
        Payment payment(evaluator.getBigger(), boost::gregorian::day_clock::local_day());
        this->paymentRepository.save(payment);
    }
}




