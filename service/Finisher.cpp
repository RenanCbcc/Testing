//
// Created by renan on 31/08/18.
//

#include <vector>
#include "Finisher.h"
#include "../persistence/AuctionDAO.h"
#include "../domain/Auction.h"

Finisher::Finisher(AuctionDAO &dao) : dao(dao) {}


void Finisher::closes() {
    // or yet auto dao = new FakeAuctionDAO(), but with mock this is not necessary anymore;
    std::vector<Auction *> allCurrentAuctions = dao.current();
    for (Auction *auction : allCurrentAuctions) {
        if (itStartedLastWeek(auction)) {
            auction->close();
            total++;
            dao.update(auction);
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




