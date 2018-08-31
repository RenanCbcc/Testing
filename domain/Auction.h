//
// Created by renan on 25/08/18.
//

#ifndef TESTING_AUCTION_H
#define TESTING_AUCTION_H


#include "User.h"
#include "Bid.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class Auction {

public:
    explicit Auction(const std::string &description);

    void propose(Bid *);

    void doubleBid(User *);

    void setDate(const boost::gregorian::date &date);

    void close();

    const boost::gregorian::date &getDate() const;

    bool isClosed();

    const std::string &getDescription() const;

    const std::vector<Bid *> &getBids() const;


private:
    std::string description;
    std::vector<Bid *> bids;

    Bid *getLastBid();

    int id;

    int quantityOfBids(const Bid *) const;
    boost::gregorian::date date;
    bool closed;
    bool mayBid(const Bid *, int);

};


#endif //TESTING_AUCTION_H
