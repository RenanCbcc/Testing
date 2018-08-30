//
// Created by renan on 25/08/18.
//

#include "Auction.h"


Auction::Auction(const std::string &description) : description(description) {}

void Auction::propose(Bid *bid) {
    int total = quantityOfBids(bid);
    if (bids.empty() || mayBid(bid, total)) {
        Auction::bids.push_back(bid);
    }

}


void Auction::doubleBid(User *user) {
    for (int i = (bids.size() - 1); i >= 0; i--) {
        if (bids[i]->getUser() == user) {
            Auction::bids.push_back(new Bid(user, bids[i]->getValue() * 2));
            return;
        }
    }


}

bool Auction::mayBid(const Bid *bid, int total) { return getLastBid()->getUser() != bid->getUser() and total < 5; }

int Auction::quantityOfBids(const Bid *bid) const {
    int total = 0;
    for (auto b: bids) {
        if (b->getUser() == bid->getUser()) {
            total++;
        }
    }
    return total;
}

Bid *Auction::getLastBid() { return bids[bids.size() - 1]; }

const std::string &Auction::getDescription() const {
    return description;
}

const std::vector<Bid *> &Auction::getBids() const {
    return bids;
}


