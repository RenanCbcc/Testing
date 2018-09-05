//
// Created by renan on 31/08/18.
//

#include <vector>
#include "Finisher.h"
#include "../persistence/AuctionDAO.h"
#include "../domain/Auction.h"

Finisher::Finisher() = default;

void Finisher::closes() {
    auto dao = new AuctionDAO();
    std::vector<Auction *> allCurrentAuctions = dao->currents();
    for (Auction *auction : allCurrentAuctions) {
        if (itStartedLastWeek(auction)) {
            auction->close();
            total++;
            dao->update(auction);
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


