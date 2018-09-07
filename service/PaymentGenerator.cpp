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

boost::gregorian::date PaymentGenerator::getBusinessDay() {
    boost::gregorian::date date = boost::gregorian::day_clock::local_day();
    if (date.day() == boost::gregorian::Saturday) {
        date += boost::gregorian::days(2);
    } else if (date.day() == boost::gregorian::Sunday) {
        date += boost::gregorian::days(1);
    }
    return date;
}




