//
// Created by renan on 31/08/18.
//

#ifndef TESTING_FINISHER_H
#define TESTING_FINISHER_H


#include "../domain/Auction.h"
#include "../persistence/AuctionDAO.h"
#include "../domain/EmailSender.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class Finisher {
public:
    /*
     Finisher class may receive a concrete DAO class or a mock derived class */
    explicit Finisher(AuctionDAO &, EmailSender &);

    void closes();

    int getTotalClosed();

private:
    int differenceDatesInDays(boost::gregorian::date);

    bool itStartedLastWeek(Auction *auction);

    int total;

    AuctionDAO &dao;
    EmailSender &mailman;
};


#endif //TESTING_FINISHER_H
