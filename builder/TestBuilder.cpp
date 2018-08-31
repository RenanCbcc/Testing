//
// Created by renan on 30/08/18.
//

#include "TestBuilder.h"

TestBuilder::TestBuilder() = default;

TestBuilder *TestBuilder::to(std::string description) {
    auction = new Auction(description);
    return this;
}

TestBuilder *TestBuilder::withBid(User *user, double value) {
    auction->propose(new Bid(user, value));
    return this;
}

Auction *TestBuilder::build() {
    return auction;
}

TestBuilder *TestBuilder::withDoubleBid(User *user) {
    auction->doubleBid(user);
    return this;
}

TestBuilder *TestBuilder::atDate(boost::gregorian::date date) {
    auction->setDate(date);
    return this;
}
