//
// Created by renan on 07/09/18.
//

#include "PaymentGenerator.h"

PaymentGenerator::PaymentGenerator(AuctionDAO &dao, Evaluator &ev, PaymentRepository &pr) :
        dao(dao), evaluator(ev), paymentRepository(pr) {}


void PaymentGenerator::generate() {
    for (auto auction: dao.closed()) {
        PaymentGenerator::evaluator.evaluate(auction);
        Payment payment(evaluator.getBigger(), getFirstBusinessDay());
        this->paymentRepository.save(payment);
    }
}

boost::gregorian::date PaymentGenerator::getFirstBusinessDay() {
    boost::gregorian::date date = {2018,7,9};//boost::gregorian::day_clock::local_day();
    if (date.day() == boost::gregorian::Saturday) {
        date += boost::gregorian::days(2);
    } else if (date.day() == boost::gregorian::Sunday) {
        date += boost::gregorian::days(1);
    }
    return date;
}




