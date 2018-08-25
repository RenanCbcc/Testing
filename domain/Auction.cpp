//
// Created by renan on 25/08/18.
//

#include "Auction.h"


Auction::Auction(const std::string &description) : description(description) {}

void Auction::propose(Bid *bid) {
    Auction::bids.push_back(bid);
}

const std::string &Auction::getDescription() const {
    return description;
}

const std::vector<Bid *> &Auction::getBids() const {
    return bids;
}
