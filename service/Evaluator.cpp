//
// Created by renan on 25/08/18.
//

#include <limits>
#include <algorithm>
#include <iostream>
#include "Evaluator.h"

Evaluator::Evaluator() {
    Evaluator::smaller = std::numeric_limits<double>::infinity();
    Evaluator::bigger = -1 * (std::numeric_limits<double>::infinity());
    Evaluator::mean = 0;
};

void Evaluator::evaluate(Auction *auction) {
    Evaluator::biggest = auction->getBids();
    std::sort(Evaluator::biggest.begin(), Evaluator::biggest.end(), std::greater<Bid *>());
    Evaluator::smaller = Evaluator::biggest[biggest.size() - 1]->getValue();
    Evaluator::bigger = Evaluator::biggest[0]->getValue();
    if (Evaluator::biggest.size() > 3) {
        std::vector<Bid *>::const_iterator first = Evaluator::biggest.begin();
        std::vector<Bid *> newVec(first, first + 3);
        Evaluator::biggest = newVec;
    }
    //TODO Free the pointer;

}

double Evaluator::getBigger() const {
    return bigger;
}

double Evaluator::getSmaller() const {
    return smaller;
}

void Evaluator::calculate(Auction *auction) {
    for (Bid *bid: auction->getBids()) {
        mean += bid->getValue();
    }
    mean = mean / auction->getBids().size();
}

double Evaluator::getMean() const {
    return mean;
}

const std::vector<Bid *> &Evaluator::getBiggest() const {
    return biggest;
}


