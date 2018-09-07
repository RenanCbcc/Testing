//
// Created by renan on 31/08/18.
//

#include <vector>
#include "Finisher.h"
#include "../persistence/AuctionDAO.h"
#include "../domain/Auction.h"

Finisher::Finisher(AuctionDAO &dao, EmailSender &emailSender) : dao(dao), mailman(emailSender) {}


void Finisher::closes() {
    for (Auction *auction : dao.current()) {
        try {
            if (itStartedLastWeek(auction)) {
                auction->close();
                total++;
                dao.update(auction);
                mailman.send(auction);
                //dao.update(auction);
            }
        } catch (std::exception &e) {
            //Do anything else.
        }
    }
}

bool Finisher::itStartedLastWeek(Auction *auction) {
    return differenceDatesInDays(auction->getDate()) >= 7;
}

int Finisher::getTotalClosed() {
    return total;
}

int Finisher::differenceDatesInDays(boost::gregorian::date date) {
    return (boost::gregorian::day_clock::local_day() - date).days();
}




