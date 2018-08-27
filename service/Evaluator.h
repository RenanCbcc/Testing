//
// Created by renan on 25/08/18.
//

#ifndef TESTING_EVALUATOR_H
#define TESTING_EVALUATOR_H


#include "../domain/Auction.h"

class Evaluator {

public:
    Evaluator();

    void evaluate(Auction *);

    void calculate(Auction *);

    double getBigger() const;

    double getMean() const;

    double getSmaller() const;

    const std::vector<Bid *> &getBiggest() const;

private:
    double bigger;
    double smaller;
    double mean;
    std::vector<Bid *> biggest;
};


#endif //TESTING_EVALUATOR_H
