//
// Created by renan on 31/08/18.
//

#ifndef TESTING_BIDDAO_H
#define TESTING_BIDDAO_H


#include <vector>
#include "../domain/Auction.h"

class AuctionDAO {
public:
    virtual void save(Auction *) = 0;

    virtual std::vector<Auction *> closed() = 0;

    virtual std::vector<Auction *> current() = 0;

    virtual void update(Auction *) = 0;

};


#endif //TESTING_BIDDAO_H
