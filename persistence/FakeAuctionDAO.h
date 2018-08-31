//
// Created by renan on 31/08/18.
//

#ifndef TESTING_FAKEAuctionDAO_H
#define TESTING_FAKEAuctionDAO_H


#include <vector>
#include "../domain/Auction.h"

class FakeAuctionDAO {
public:

    virtual void save(Auction *);

    virtual std::vector<Auction *> closed();

    virtual std::vector<Auction *> current();

    virtual void update(Auction *); /* does nothing! */

private:
    std::vector<Auction *> Auctions;
};


#endif //TESTING_FAKEAuctionDAO_H
