//
// Created by renan on 31/08/18.
//

#ifndef TESTING_FAKEAuctionDAO_H
#define TESTING_FAKEAuctionDAO_H


#include <vector>
#include "../domain/Auction.h"

class FakeAuctionDAO {
public:

    void save(Auction *);

    std::vector<Auction *> closed();

    std::vector<Auction *> current();

    void update(Auction *); /* does nothing! */

private:
    std::vector<Auction *> Auctions;
};


#endif //TESTING_FAKEAuctionDAO_H
