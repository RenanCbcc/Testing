//
// Created by renan on 25/08/18.
//

#ifndef TESTING_EVALUATOR_H
#define TESTING_EVALUATOR_H


#include "../domain/Auction.h"

class Evaluator {
public:
    void evaluate(Auction *);

    double getBigger() const;

    double getSmaller() const;

private:
    double bigger;
    double smaller;
};


#endif //TESTING_EVALUATOR_H
