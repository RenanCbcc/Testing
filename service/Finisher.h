//
// Created by renan on 31/08/18.
//

#ifndef TESTING_FINISHER_H
#define TESTING_FINISHER_H


#include "../domain/Auction.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class Finisher {
public:
    Finisher();

    void closes();

    int getTotalClosed();

private:
    int differenceDatesInDays(boost::gregorian::date);

    bool itStartedLastWeek(Auction *auction);

    int total;
};


#endif //TESTING_FINISHER_H
