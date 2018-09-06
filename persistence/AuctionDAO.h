//
// Created by renan on 31/08/18.
//

#ifndef TESTING_BIDDAO_H
#define TESTING_BIDDAO_H


#include <vector>
#include "../domain/Auction.h"

class AuctionDAO {
public:
    virtual void save(Auction *);

    virtual void update(Auction *);

    virtual std::vector<Auction *> closed();

    virtual std::vector<Auction *> current();


};


#endif //TESTING_BIDDAO_H
