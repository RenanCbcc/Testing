//
// Created by renan on 25/08/18.
//

#include <limits>
#include "Evaluator.h"

void Evaluator::evaluate(Auction *auction) {
    for (Bid *bid: auction->getBids()) {
        if (bid->getValue() > Evaluator::bigger) {
            Evaluator::bigger = bid->getValue();
        }
        if (bid->getValue() < Evaluator::smaller) {
            Evaluator::smaller = bid->getValue();
        }
    }
}

double Evaluator::getBigger() const {
    return bigger;
}

double Evaluator::getSmaller() const {
    return smaller;
}

Evaluator::Evaluator() {
    Evaluator::smaller = std::numeric_limits<double>::infinity();
    Evaluator::bigger = -1 * (std::numeric_limits<double>::infinity());
};
