//
// Created by renan on 25/08/18.
//

#ifndef TESTING_AUCTION_H
#define TESTING_AUCTION_H


#include "User.h"
#include "Bid.h"

class Auction {

public:
    explicit Auction(const std::string &description);
    void propose(Bid *);

    const std::string &getDescription() const;

    const std::vector<Bid *> &getBids() const;

private:
    std::string description;
    std::vector<Bid *> bids;

};


#endif //TESTING_AUCTION_H
